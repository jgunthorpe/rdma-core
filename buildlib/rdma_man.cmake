# COPYRIGHT (c) 2017 Mellanox Technologies Ltd
# Licensed under BSD (MIT variant) or GPLv2. See COPYING.

function(rdma_md_man_page SRC MAN_SECT MANFN)
  set(OBJ "${CMAKE_CURRENT_BINARY_DIR}/${MANFN}")

  add_custom_command(
    OUTPUT "${OBJ}"
    COMMAND "${PANDOC_EXECUTABLE}" -s -t man "${SRC}" -o "${OBJ}"
    MAIN_DEPENDENCY "${SRC}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Creating man page ${MANFN}"
    VERBATIM)

  add_custom_target("man-${MANFN}.${MAN_SECT}" ALL DEPENDS "${OBJ}")
  install(FILES "${OBJ}"
    DESTINATION "${CMAKE_INSTALL_MANDIR}/man${MAN_SECT}/")
endfunction()

# Install man pages. This deduces the section from the trailing integer in the
# filename
function(rdma_man_pages)
  foreach(I ${ARGN})
    if ("${I}" MATCHES "\\.md.in$")
      string(REGEX REPLACE "^.+[.](.+)\\.md.in$" "\\1" MAN_SECT "${I}")
      string(REGEX REPLACE "^(.+)\\.md.in$" "\\1" BASE_NAME "${I}")
      get_filename_component(BASE_NAME "${BASE_NAME}" NAME)

      configure_file("${I}" "${CMAKE_CURRENT_BINARY_DIR}/sub-${BASE_NAME}" @ONLY)
      rdma_md_man_page(
	"${CMAKE_CURRENT_BINARY_DIR}/sub-${BASE_NAME}"
	"${MAN_SECT}"
	"${BASE_NAME}")
    elseif ("${I}" MATCHES "\\.md$")
      string(REGEX REPLACE "^.+[.](.+)\\.md$" "\\1" MAN_SECT "${I}")
      string(REGEX REPLACE "^(.+)\\.md$" "\\1" BASE_NAME "${I}")
      get_filename_component(BASE_NAME "${BASE_NAME}" NAME)

      rdma_md_man_page(
	"${I}"
	"${MAN_SECT}"
	"${BASE_NAME}")
    elseif ("${I}" MATCHES "\\.in$")
      string(REGEX REPLACE "^.+[.](.+)\\.in$" "\\1" MAN_SECT "${I}")
      string(REGEX REPLACE "^(.+)\\.in$" "\\1" BASE_NAME "${I}")
      get_filename_component(BASE_NAME "${BASE_NAME}" NAME)
      rdma_subst_install(FILES "${I}"
	DESTINATION "${CMAKE_INSTALL_MANDIR}/man${MAN_SECT}/"
	RENAME "${BASE_NAME}")
    else()
      string(REGEX REPLACE "^.+[.](.+)$" "\\1" MAN_SECT "${I}")
      install(FILES "${I}" DESTINATION "${CMAKE_INSTALL_MANDIR}/man${MAN_SECT}/")
    endif()
  endforeach()
endfunction()

# Create an alias for a man page, using a symlink.
# Input is a list of pairs of names (MAN_PAGE ALIAS)
# NOTE: The section must currently be the same for both.
function(rdma_alias_man_pages)
  list(LENGTH ARGN LEN)
  math(EXPR LEN ${LEN}-1)
  foreach(I RANGE 0 ${LEN} 2)
    list(GET ARGN ${I} FROM)
    math(EXPR I ${I}+1)
    list(GET ARGN ${I} TO)
    string(REGEX REPLACE "^.+[.](.+)$" "\\1" MAN_SECT ${FROM})
    rdma_install_symlink("${FROM}" "${CMAKE_INSTALL_MANDIR}/man${MAN_SECT}/${TO}")
  endforeach()
endfunction()
