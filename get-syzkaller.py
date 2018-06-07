#!/usr/bin/env python2.7
from __future__ import print_function
import collections
import re
import copy
from clang.cindex import Index
from clang.cindex import CursorKind
from clang.cindex import TypeKind

all_enums = {};

class TypeOverride(object):
    def __init__(self):
        self.overrides = [];
        self.bitfields = {};

    def translate(self,field,record_type):
        field = "%s::%s"%(record_type.spelling,field.spelling)
        for I in self.overrides:
            if re.match(I[0],field) is not None:
                return I[1];
        return None;

    def add(self,match,result,require_type=None):
        """Add a general overide, when struct::field is matched to regex *match* the
        type is switched ot *result*"""
        # FIXME: sanity check that require_type matches the struct field type
        self.overrides.append((match,result));

    def bitfield_name(self,enum_cursor):
        return "enum_%s"%(enum_cursor.spelling);

    def add_flags(self,match,enum_cursor):
        """Like add, except the type is switched to a bitfiled, with
        valid values specified by the enum referenced by *enum_cursor*"""
        self.add(match,"flags[%s, {type}]"%(self.bitfield_name(enum_cursor)));
        self.bitfields[enum_cursor.spelling] = enum_cursor;

global_override = TypeOverride();

# Define special structure members
idr_handles = {"pd_handle","ah_handle","mr_handle","cq_handle","qp_handle","mw_handle","srq_handle",
               "xrcd_handle","wq_handle","ind_tbl_handle","flow_handle"};
for I in sorted(idr_handles):
    print("resource %s[int32]"%(I));
    global_override.add(".*::%s"%(I),I,"int32");

global_override.add(".*::.*cq_handle","cq_handle","int32");

# Note the syzkaller 'fd' type must be int32 compatible
print("resource fd_comp_chan[fd]")
global_override.add(".*ib_uverbs_create_comp_channel_resp::fd","fd_comp_chan","int32");

print("resource fd_async[fd]")
global_override.add(".*ib_uverbs_get_context_resp::async_fd","fd_async","int32");

print("resource qp_number[int32]")
global_override.add(".*ib_uverbs_create_qp_resp::qpn","qp_number","int32");
global_override.add(".*ib_uverbs_open_qp::qpn","qp_number","int32");

print("resource mr_rkey[int32]");
global_override.add(".*::rkey","mr_rkey","int32");
print("resource mr_lkey[int32]");
global_override.add(".*::lkey","mr_lkey","32");

# The uverbs FD itself
print("resource fd_rdma[fd]")

def get_comment(cursor):
    """Read the comment trailing an expression."""
    src = cursor.extent.end;
    with open(src.file.name) as F:
        F.seek(src.offset);
        ln = F.readline();
    g = re.search(r"/\*(.*)\*/",ln);
    if g is None:
        return None;
    return g.group(1).strip();

class WriteAPI(object):
    full_req = None;
    req = None;
    resp = None;
    drv_req = None;
    drv_resp = None;

    def __init__(self):
        self.override = global_override;

    def set_type(self,kind,cursor):
        ty = cursor.type.get_canonical();
        assert ty.kind == TypeKind.RECORD;
        if ty.spelling == "struct empty":
            return;
        setattr(self,kind,ty);

    def load_flags(self,record_type):
        """Check all the fields of a record for trailing comments indicating it is
        linked to an enum"""
        if record_type is None:
            return;

        for I in record_type.get_fields():
            comment = get_comment(I);
            if not comment:
                continue;
            g = re.search(r"enum (\S+)",comment);
            if g is None:
                continue;
            cursor = all_enums.get(g.group(1))
            if cursor is None:
                print("# WARNING: enum %r in comment %r of %s::%s not found"%(
                    g.group(1),comment,record_type.spelling,I.spelling))
                return;
            # FIXME: We should look at the num and decide if it is a bitfiled
            # or something else..
            global_override.add_flags("%s::%s"%(record_type.spelling,
                                                I.spelling),
                                      cursor);

    def cleanup(self,enum_name,enum_value):
        """The C macros use 'struct empty' to indicate there is no
        struct here. Convert to None for consistency"""
        self.enum_name = enum_name;
        self.enum_value = enum_value;

        # Figure out what the header struct is
        for I in self.full_req.get_fields():
            assert I.spelling == "hdr";
            self.hdr = I.type.get_canonical();
            assert (self.hdr.spelling == "struct ex_hdr" or
                    self.hdr.spelling == "struct ib_uverbs_cmd_hdr");
            self.is_ex = self.hdr.spelling == "struct ex_hdr";
            break;

        self.total_req_bytes = self.full_req.get_size();
        if self.resp is not None:
            self.total_resp_bytes = self.resp.get_size();
        else:
            self.total_resp_bytes = 0;

        if self.drv_req is not None:
            self.total_req_bytes += self.drv_req.get_size();
        if self.drv_resp is not None:
            self.total_resp_bytes += self.drv_resp.get_size();

        self.load_flags(self.req);
        self.load_flags(self.resp);
        self.load_flags(self.drv_req);
        self.load_flags(self.drv_resp);

    def syzkaller_req_name(self):
        if self.drv_req is None and self.drv_resp is None:
            return "%s_core"%(self.enum_name);
        return "%s_%s"%(self.enum_name,drv_name);

    def syzkaller_resp_name(self):
        if self.drv_req is None and self.drv_resp is None:
            return "%s_core_resp"%(self.enum_name);
        return "%s_%s_resp"%(self.enum_name,drv_name);

    syz_type = {
        "__u64": "int64",
        "__u32": "int32",
        "__u16": "int16",
        "__be64": "int64be",
        "__be32": "int32be",
        "__be16": "int16be",
        "__u8": "int8",
        "__s64": "int64",
        "__s32": "int32",
        "__s16": "int16",
        };
    def field_to_syzkaller(self,cursor,record_type,prefix=""):
        syz_name = prefix + cursor.spelling;
        type = cursor.type;
        trans = self.override.translate(cursor,record_type);
        if not trans:
            trans = "{type}";

        #print(cursor.spelling,cursor.kind,cursor.type.get_named_type())
        if type.kind == TypeKind.TYPEDEF:
            typedef_name = type.get_typedef_name();
            # Turns out all typedef'd things are simple names.
            if "reserved" in cursor.spelling:
                syz_type = "const[0x0,%s]"%(self.syz_type[typedef_name]);
            else:
                syz_type = "%s"%(self.syz_type[typedef_name]);
        elif type.kind == TypeKind.CONSTANTARRAY:
            typedef_name = type.get_array_element_type().get_typedef_name();

            # FIXME: Not all flex arrays should be suppressed, only the core ones..
            if type.get_array_size() == 0:
                print("\t#",end="");

            if "reserved" in cursor.spelling:
                syz_type = "const[0, array[%s,%u]]"%(
                    self.syz_type.get(typedef_name,typedef_name),
                    type.get_array_size())
            else:
                syz_type = "array[%s,%u]"%(
                    self.syz_type.get(typedef_name,typedef_name),
                    type.get_array_size())
        elif type.kind == TypeKind.ELABORATED:
            ty = cursor.type.get_canonical();
            print("\t# Nested under %s"%(cursor.spelling));
            self.fields_to_syzkaller(ty,prefix=syz_name+"_");
            return;
        else:
            print("??",cursor.spelling,type.spelling,type.kind);
            return;
        print("\t%s %s"%(syz_name,trans.format(type=syz_type)));

    def fields_to_syzkaller(self,record_type,**kwargs):
        """Each element in the list fields is a cursor to the struct"""
        if record_type is None:
            return;
        assert record_type.kind == TypeKind.RECORD;
        print("\t# Type %s"%(record_type.spelling));
        for I in record_type.get_fields():
            self.field_to_syzkaller(I,record_type=record_type,**kwargs);

    def req_as_syzkaller(self):
        """Render the clang data into a syzkaller description for the write() request
        struct"""
        print("%s {"%(self.syzkaller_req_name()));

        # we could read these types from the structs, but this is simpler..
        if not self.is_ex:
            assert(self.total_req_bytes % 4 == 0);
            assert(self.total_resp_bytes % 4 == 0);
            print("\tcommand const[0x%x, int32]"%(self.enum_value));
            print("\tin_words const[0x%x, int16]"%(self.total_req_bytes//4));
            print("\tout_words const[0x%x, int16]"%(self.total_resp_bytes//4));
            if self.resp or self.drv_resp:
                self.override = copy.copy(self.override);
                self.override.overrides = copy.copy(self.override.overrides)
                self.override.add("%s::response"%(self.req.spelling),
                                  "ptr64[out, %s]"%(self.syzkaller_resp_name()));
        else:
            assert(self.total_req_bytes % 8 == 0);
            assert(self.total_resp_bytes % 8 == 0);
            print("\tcommand const[0x%x, int32]"%(self.enum_value));
            print("\tin_words [0x%x, int16]"%(self.total_req_bytes//8));
            print("\tout_words [0x%x, int16]"%(self.total_resp_bytes//8));
            print("\tresponse ptr64[out, %s]"%(self.syzkaller_resp_name()));
            words = self.drv_req.get_size() if self.drv_req is not None else 0;
            print("\tprovider_in_words [0x%x, int16]"%(words//8));
            words = self.drv_resp.get_size() if self.drv_resp is not None else 0;
            print("\tprovider_out_words [0x%x, int16]"%(words/8));
            print("\tcmd_hdr_reserved const[0, int32]");
        self.fields_to_syzkaller(self.req);
        self.fields_to_syzkaller(self.drv_req);
        print("}");

    def resp_as_syzkaller(self):
        """Render the clang data into a syzkaller description for the write() request
        struct"""
        print("%s {"%(self.syzkaller_resp_name()));
        self.fields_to_syzkaller(self.resp);
        self.fields_to_syzkaller(self.drv_resp);
        print("}");

def read_structs(unit):
    all_enum_values = {};
    apis = collections.defaultdict(WriteAPI);
    for I in unit.cursor.get_children():
        if I.kind == CursorKind.ENUM_DECL:
            all_enums[I.spelling] = I;
            for cursor in I.get_children():
                all_enum_values[cursor.spelling] = cursor;

        if I.kind != CursorKind.TYPEDEF_DECL:
            continue;

        for regex,kind in ((r"_ABI_REQ_STRUCT_(IB_USER_.*)","full_req"),
                           (r"_KABI_REQ_STRUCT_(IB_USER_.*)","req"),
                           (r"_KABI_RESP_STRUCT_(IB_USER_.*)","resp"),
                           (r"_KABI_DRV_REQ_STRUCT_(IB_USER_.*)","drv_req"),
                           (r"_KABI_DRV_RESP_STRUCT_(IB_USER_.*)","drv_resp"),
                           ("_ABI_REQ_STRUCT_(IB_USER_.*)","full_req")):
            g = re.match(regex,I.spelling)
            if g is not None:
                apis[g.group(1)].set_type(kind,I);

    for k,v in apis.items():
        v.cleanup(k,all_enum_values[k].enum_value);

    return apis;

clang_args = ["-I","build/include"];
index = Index.create();

# FIXME, hard wired for now
drv_name = "mlx5";
unit = index.parse("providers/mlx5/mlx5-abi.h",
                   args=clang_args);
apis = read_structs(unit);

print();
for name,cursor in sorted(global_override.bitfields.items()):
    bits = [];
    for I in cursor.get_children():
        bits.append("0x%x"%(I.enum_value));
    print("%s = %s"%(global_override.bitfield_name(cursor),
                     ",".join(bits)));
print();
for k,v in sorted(apis.items()):
    v.resp_as_syzkaller();
    print();
    v.req_as_syzkaller();
    print("write$%s(fd fd_rdma, buf ptr[inout, %s], len len[buf])"%(
        v.syzkaller_req_name(), v.syzkaller_req_name()));
    print()
