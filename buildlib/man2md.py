#!/usr/bin/env python3
from __future__ import print_function
import re
import sys
import textwrap
import tempfile
import subprocess
import os
import yaml
import functools

clang_format_synopsis = {
    "BasedOnStyle": "LLVM",
    "IndentWidth": 8,
    "BreakBeforeBraces": "Linux",
    "AllowShortIfStatementsOnASingleLine": False,
    "IndentCaseLabels": False,
    "UseTab": "Never",
    "BinPackParameters": False,
    "ColumnLimit": 66,
    };

clang_format_body = {
    "BasedOnStyle": "LLVM",
    "IndentWidth": 8,
    "BreakBeforeBraces": "Linux",
    "AllowShortIfStatementsOnASingleLine": False,
    "IndentCaseLabels": False,
    "UseTab": "Never",
    "BinPackParameters": False,
    "AlignConsecutiveDeclarations": True,
    "AlignTrailingComments": True,
    "ColumnLimit": 66,
    };

class BadSyntax(Exception):
    pass;

class Context(object):
    def __init__(self):
        self.metadata = {
            "layout": "page",
        };
        self.data = [];
        self.synopsis = [];
        self.see_also = set();
        self.author = set();

        self.cur_matches = [self.top_matches];

    def unquote(self,s):
        j = re.findall(r'([^" ]\S*|".*?")', s)
        for I in j:
            if I[0] == '"':
                assert I[-1] == '"';
        return " ".join(I[1:-1] if I[0] == '"' else I for I in j);

    def unescape(self,s):
        # FIXME
        s = s.replace("\\-","-");
        s = s.replace("\\c","");
        s = re.sub(r"\\fB([^\\]+)\\fR",r"**\1**",s);
        s = re.sub(r"\\fI([^\\]+)\\fR",r"*\1*",s);
        s = re.sub(r"\\s-1([^\\]+)\\s0",r"\1",s);
        if '\\' in s:
            raise BadSyntax("Didn't remove escape %r"%(s));
        return s;

    # ----------------------------------------------------------------

    def handle_license(self,g):
        # Of this file
        self.metadata["license"] = g.group(1);

    def handle_header(self,g):
        self.metadata["title"] = g.group(1);
        self.metadata["section"] = g.group(2);
        self.metadata["date"] = g.group(3);
        self.metadata["footer"] = g.group(4);
        if len(g.groups()) >= 5:
            self.metadata["header"] = g.group(5);

    def skip(self,g):
        pass

    def handle_section_header(self,g):
        next_section = self.sections.get(g.group(1),self.section_matches);
        if len(self.cur_matches) == 1:
            self.cur_matches.append(next_section);
        else:
            self.cur_matches[-1] = next_section;
            self.tidy_last_para();
            self.data.append("");
        self.data.append("# %s"%(g.group(1)));
        self.data.append("");

    top_matches = [
        (r"\.\\\" (Licensed.*)",handle_license),
        (r"\.\\\" *", skip),
        (r'\.TH (.*) (\d+) (.*) (.*) "(.*)"', handle_header),
        (r'\.TH (.*) (\d+) (.*) (.*)', handle_header),
        (r'\.SH "(.*)"', handle_section_header),
        (r'\.SH (.*)', handle_section_header),
        ];

    # ----------------------------------------------------------------

    def handle_free_text(self,g):
        self.data.append(self.unescape(g.group(0)));

    def handle_bold_text(self,g):
        self.data.append("**" + self.unescape(g.group(1)) + "**");

    def handle_bold_text2(self,g):
        self.data.append("**" + self.unescape(g.group(1)) + "**" +
                         self.unescape(g.group(2)));

    def handle_italic_text(self,g):
        self.data.append("*" + self.unescape(g.group(1)) + "*");

    def handle_italic_text2(self,g):
        self.data.append("*" + self.unescape(g.group(1)) + "*" +
                         self.unescape(g.group(2)));

    def handle_para_break(self,g):
        self.tidy_last_para();
        self.data.append("");

    def handle_code_block(self,g):
        self.synopsis = [];
        self.cur_matches.append(self.code_block_matches);
        self.tidy_last_para();
        self.data.append("");

    def handle_definition_list(self,g):
        self.cur_matches.append(self.definition_list_matches_first);

    def handle_sub_section_header(self,g):
        next_section = self.sections.get(g.group(1),self.section_matches);
        if len(self.cur_matches) == 1:
            self.cur_matches.append(next_section);
        else:
            self.cur_matches[-1] = next_section;
            self.tidy_last_para();
            self.data.append("");
        self.data.append("## %s"%(g.group(1)));
        self.data.append("");

    section_matches = [
        (r'\.SH "(.*)"', handle_section_header),
        (r'\.SH (.*)', handle_section_header),

        (r'\.SS "(.*)"', handle_sub_section_header),
        (r'\.SS (.*)', handle_sub_section_header),

        (r'[^.].*', handle_free_text),
        (r'\.B (.*)\\fR(.*)', handle_bold_text2),
        (r'\.B (.*)', handle_bold_text),
        (r'\.BR (.*)', handle_bold_text),
        (r'\.I (.*)\\fB\\fR(.*)', handle_italic_text2),
        (r'\.I (.*)\\fR(.*)', handle_italic_text2),
        (r'\.I (.*)', handle_italic_text),
        (r'\.IR (.*?) (.*)', handle_italic_text2),
        (r'\.IR (.*)', handle_italic_text),
        (r'\.PP', handle_para_break),
        (r'\.sp', handle_para_break),
        (r'\.br', handle_para_break),

        (r'\.nf', handle_code_block),

        (r'\.TP', handle_definition_list),
    ];

    # ----------------------------------------------------------------
    def handle_def_label(self,g,wrap):
        self.data.append(wrap + self.unescape(g.group(1)) + wrap);
        self.cur_matches[-1] = self.definition_list_matches_second;
        self.definition_text = [];

    def handle_def_label_and(self,g,wrap):
        self.data.append(wrap + self.unescape(g.group(1)) + wrap);
        self.cur_matches[-1] = self.definition_list_matches_second;
        self.definition_text = [self.unescape(g.group(2)).strip()];

    definition_list_matches_first = [
        (r'\.B (.*?)\\fR(.*)', functools.partial(handle_def_label_and,wrap="**")),
        (r'\.B (.*)', functools.partial(handle_def_label,wrap="**")),
        ];

    def handle_def_text(self,g,wrap=""):
        self.definition_text.append(wrap + self.unescape(g.group(1)) + wrap);

    def handle_def_text_final(self,g):
        self.handle_def_text(g);
        self.handle_done_definition_list(None);

    def flush_definition(self):
        lines = textwrap.wrap(" ".join(self.definition_text),width=78-8);
        for idx,I in enumerate(lines):
            if idx == 0:
                self.data.append(":\t"+I);
            else:
                self.data.append("\t"+I);
        self.data.append("");

    def handle_next_definition_list(self,g):
        self.flush_definition();
        self.cur_matches[-1] = self.definition_list_matches_first;

    def handle_done_definition_list(self,g):
        self.flush_definition();
        del self.cur_matches[-1];

    def handle_done_definition_list_again(self,g):
        self.flush_definition();
        del self.cur_matches[-1];
        self.handle_ln(g.group(0));

    definition_list_matches_second = [
        (r'([^.].*)\\fR', handle_def_text_final),
        (r'([^.].*)', handle_def_text),
        (r'\.BR (.*)', functools.partial(handle_def_text,wrap="**")),
        (r'\.BI (.*)', functools.partial(handle_def_text,wrap="*")),
        (r'\.I (.*)', functools.partial(handle_def_text,wrap="*")),
        (r'\.PP', handle_done_definition_list),
        (r'\.TP', handle_next_definition_list),
        (r'\.SH "(.*)"', handle_done_definition_list_again),
        ];

    # ----------------------------------------------------------------

    def handle_author(self,g):
        self.author.add(self.unescape(g.group(0)));

    def done_author(self,g):
        assert self.data[-2] == "# AUTHOR" or self.data[-2] == "# AUTHORS";

        # Correct plurality of the author heading
        if len(self.author) == 0:
            del self.data[-2:];
        elif len(self.author) == 1:
            self.data[-2] = "# AUTHOR";
        else:
            self.data[-2] = "# AUTHORS";

        for idx,I in enumerate(sorted(self.author)):
            if idx+1 == len(self.author):
                self.data.append("%s"%(I,));
            else:
                self.data.append("%s,"%(I,));

        del self.cur_matches[-1];
        self.data.append("");
        if g is not None:
            self.handle_section_header(g);

    section_matches_author = [
        (r'\.SH "(.*)"', done_author),
        (r'\.SH (.*)', done_author),
        (r'\.TP', skip),
        (r'[^.].*', handle_author),
    ];

    # ----------------------------------------------------------------

    def handle_sa_link(self,g):
        self.see_also.add((g.group(1),int(g.group(2))));

    def done_sa(self,g):
        for idx,I in enumerate(sorted(self.see_also)):
            if idx+1 == len(self.see_also):
                self.data.append("**%s**(%u)"%I);
            else:
                self.data.append("**%s**(%u),"%I);

        del self.cur_matches[-1];
        self.data.append("");
        self.handle_section_header(g);

    section_matches_see_also = [
        (r'\.SH "(.*)"', done_sa),
        (r'\.SH (.*)', done_sa),
        (r'\.BR (.*) \((\d+)\)', handle_sa_link),
    ];

    # ----------------------------------------------------------------

    def handle_empty_line(self,g):
        self.synopsis.append("");

    def handle_verbatim_data(self,g):
        self.synopsis.append(self.unquote(g.group(1)));

    def handle_verbatim_end(self,g):
        if self.cur_matches[-1] == self.synopsis_matches:
            style = clang_format_synopsis;
        else:
            style = clang_format_body;
        del self.cur_matches[-1];

        self.data.append("```c");
        self.data.extend(self.tidy_c_source(self.synopsis,style));
        self.data.append("```");
        self.data.append("");

    synopsis_matches = [
        (r'\.nf', skip),
        (r'\.fi', handle_verbatim_end),
        (r'\.sp', handle_empty_line),
        (r'\.BI (.*)', handle_verbatim_data),
        (r'\.B (.*)', handle_verbatim_data),
    ];

    # ----------------------------------------------------------------

    code_block_matches = [
        (r'\.nf', skip),
        (r'\.fi', handle_verbatim_end),
        (r'\.in \+\d+', skip),
        (r'\.in -\d+', skip),
        (r'\.sp', handle_empty_line),
        (r'\.PP', handle_empty_line),
        (r'(.*)', handle_verbatim_data),
    ];

    # ----------------------------------------------------------------

    sections = {
        "AUTHORS": section_matches_author,
        "AUTHOR": section_matches_author,
        "SEE ALSO": section_matches_see_also,
        "SYNOPSIS": synopsis_matches,
    };

    def handle_ln(self,ln):
        for r,fn in self.cur_matches[-1]:
            g = re.match(r,ln);
            if g:
                fn(self,g);
                return;
        #print("FAILED",repr(ln))
        raise BadSyntax("FAILED " + repr(ln));

    def tidy_last_para(self):
        """Finish off writing a paragraph by replacing the last lines with a word wrapped version"""
        for pos,I in enumerate(reversed(self.data)):
            if I == "":
                break;
            if I.startswith("'''") or I.startswith("# ") or I.startswith(":"):
                pos = pos + 1;
                break;

        if pos == 0:
            return;

        para = self.data[-pos:];
        for I in para:
            assert not I.startswith("#");
        del self.data[-pos:];
        para = " ".join(para);
        self.data.extend(textwrap.wrap(para,width=78));

    def tidy_c_source(self,lns,params):
        """Call clang-format on verbatim text which is C source code"""
        style = yaml.dump(params);
        with tempfile.NamedTemporaryFile("wt",suffix=".c",dir=os.getcwd()) as F:
            F.write("\n".join(lns));
            F.flush();
            return [I.decode() for I in subprocess.check_output([
                "clang-format-5.0",
                "--style="+style,
                F.name]).splitlines()];

    def write_md(self,F):
        """Write the final MD document to file F"""
        self.cur_matches[-1][0][1](self,None);

        while self.data[-1] == '':
            del self.data[-1];

        print("---",file=F)
        for k,v in sorted(self.metadata.items()):
            if ' ' in v:
                print("%s: %r"%(k,v),file=F);
            else:
                print("%s: %s"%(k,v),file=F);
        print("---",file=F)
        print(file=F);
        for I in self.data:
            print(I,file=F)

def convert_one(I,base):
    print("Doing",I);

    ctx = Context();
    with open(I) as F:
        for ln in F.readlines():
            ctx.handle_ln(ln.strip())
    with open(I + ".md","w") as FO:
        ctx.write_md(FO);

    with tempfile.NamedTemporaryFile() as F:
        subprocess.check_call(["pandoc","-t","man","-s",I + ".md","-o",F.name]);
        with open(os.path.join("new",base),"w") as FO:
            subprocess.check_call(["man","-l",F.name],stdout=FO);

    with open(os.path.join("original",base),"w") as FO:
        subprocess.check_call(["man","-l",I],stdout=FO);

def do_directory(dfn):
    for I in os.listdir(dfn):
        if I.endswith(".md") or I.endswith(".txt"):
            continue;
        try:
            convert_one(os.path.join(dfn,I),I);
        except BadSyntax as err:
            print("FAILED",I,err);

if sys.argv[1][-1] == '/':
    do_directory(sys.argv[1]);
else:
    ctx = Context();
    with open(sys.argv[1]) as F:
        for ln in F.readlines():
            ctx.handle_ln(ln.strip())

    ctx.write_md(sys.stdout);
