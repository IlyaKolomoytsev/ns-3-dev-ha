set(source_files
    model/ha-phy.cc
    model/ha-header.cc
    model/ha-mpdu.cc
)

set(header_files
    model/ha-phy.h
    model/ha-header.h
    model/ha-mpdu.h
    model/ha-units.h
    model/ha-phy-common.h
)

build_lib(
    LIBNAME ha
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    LIBRARIES_TO_LINK
    ${libenergy}
    ${libspectrum}
    ${gsl_libraries}
    GENERATE_EXPORT_HEADER
)
