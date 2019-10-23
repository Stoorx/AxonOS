
# Use to get mangled filename
function(MangleFilenameToObj sourceFilename objectFilename)
    get_property(suffix GLOBAL PROPERTY ObjSuffix)
    string(APPEND ${sourceFilename} ${suffix})
    set(${objectFilename} "${sourceFilename}.${suffix}" PARENT_SCOPE)
endfunction()

function(MangleFilenameListToObj sourceFiles objectFiles)
    get_property(suffix GLOBAL PROPERTY ObjSuffix)
    foreach (file IN LISTS ${sourceFiles})
        list(APPEND objectFiles "${file}${suffix}")
    endforeach ()
    set(${objectFiles} ${${objectFiles}} PARENT_SCOPE)
endfunction()

function(CompileAsm asmFile outFile format extraFlags target)
    get_property(asmCompiler GLOBAL PROPERTY AsmCrossCompiler)
    get_property(formatFlag GLOBAL PROPERTY AsmCrossCompilerOutputFormatFlag)
    get_filename_component(outFileName ${outFile} NAME)
    add_custom_command(
            OUTPUT ${outFile}
            COMMAND ${asmCompiler} ${formatFlag} ${format} ${extraFlags} -o ${outFile} ${asmFile}
            MAIN_DEPENDENCY ${asmFile}
    )
    string(MD5 pathHash ${CMAKE_CURRENT_BINARY_DIR}/${outFile})
    string(SUBSTRING ${pathHash} 0 8 pathHash)
    set(thisTarget "${outFileName}")
    set(${target} ${thisTarget} PARENT_SCOPE)
    add_custom_target(${thisTarget} DEPENDS ${outFile} SOURCES ${asmFile})
endfunction()

function(CompileCxx srcFile outFile extraFlags target)
    get_property(compiler GLOBAL PROPERTY CxxCrossCompiler)
    get_property(flags GLOBAL PROPERTY CxxCrossCompilerFlags)
    get_property(standaloneFlags GLOBAL PROPERTY CxxStandaloneFlags)
    get_filename_component(outFileName ${outFile} NAME)
    add_custom_command(
            OUTPUT ${outFile}
            COMMAND ${compiler} -c ${flags} ${standaloneFlags} ${extraFlags} -o ${outFile} ${srcFile}
            MAIN_DEPENDENCY ${srcFile}
    )
    string(MD5 pathHash ${CMAKE_CURRENT_BINARY_DIR}/${outFile})
    string(SUBSTRING ${pathHash} 0 8 pathHash)
    set(thisTarget "${outFileName}")
    set(${target} ${thisTarget} PARENT_SCOPE)
    add_custom_target(${thisTarget} DEPENDS ${outFile} SOURCES ${srcFile})
endfunction()

function(Objcopy inFile outFile format extraFlags target)
    get_property(objcopy GLOBAL PROPERTY ObjcopyExecutable)
    get_filename_component(outFileName ${outFile} NAME)
    add_custom_command(
            OUTPUT ${outFile}
            COMMAND ${objcopy} -O ${format} ${extraFlags} ${inFile} ${outFile}
            MAIN_DEPENDENCY ${inFile}
    )
    string(MD5 pathHash ${CMAKE_CURRENT_BINARY_DIR}/${outFile})
    string(SUBSTRING ${pathHash} 0 8 pathHash)
    set(thisTarget "${outFileName}")
    set(${target} ${thisTarget} PARENT_SCOPE)
    add_custom_target(${thisTarget} DEPENDS ${outFile} SOURCES ${inFile})
endfunction()

function(LinkerS inFiles outFile script extraFlags target)
    get_property(linker GLOBAL PROPERTY LinkerCrossExecutable)
    get_filename_component(outFileName ${outFile} NAME)
    add_custom_command(
            OUTPUT ${outFile}
            COMMAND ${linker} -T ${script} ${extraFlags} -o ${outFile} ${${inFiles}}
            DEPENDS ${${inFiles}} ${script}
    )
    string(MD5 pathHash ${CMAKE_CURRENT_BINARY_DIR}/${outFile})
    string(SUBSTRING ${pathHash} 0 8 pathHash)
    set(thisTarget "${outFileName}")
    set(${target} ${thisTarget} PARENT_SCOPE)
    add_custom_target(${thisTarget} DEPENDS ${outFile} SOURCES ${script})
endfunction()

function(MakeAbsolutePath inFile outFile)
    set(${outFile} ${CMAKE_CURRENT_SOURCE_DIR}/${inFile} PARENT_SCOPE)
endfunction()

function(MakeAbsolutePathList inFiles outFiles)
    foreach (file IN LISTS ${inFiles})
        list(APPEND outFiles ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    endforeach ()
    set(${outFiles} ${outFiles} PARENT_SCOPE)
endfunction()

function(PrintList list)
    foreach (e IN LISTS ${list})
        message(${e})
    endforeach ()
endfunction()

function(MapInsert key value)
    set(${key} ${value} PARENT_SCOPE)
endfunction()

function(MapMap keys values)
    set(localValues "")
    foreach (k IN LISTS ${keys})
        list(APPEND localValues ${${k}})
    endforeach ()
    set(${values} ${localValues} PARENT_SCOPE)
endfunction()

