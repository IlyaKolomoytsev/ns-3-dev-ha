build_lib_example(
    NAME ha-interference-helper
    SOURCE_FILES interference-helper.cc
    LIBRARIES_TO_LINK
    ${libha}
    ${libcore}
    ${libspectrum}
    ${libnetwork}
)
