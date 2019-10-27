
CreateBootSectorTarget(MBR.bin
        ${CMAKE_CURRENT_SOURCE_DIR}/MBR.asm)
CreateBootSectorTarget(VBR.bin
        ${CMAKE_CURRENT_SOURCE_DIR}/VBR_FAT32.asm)
AddFileToDist(VBR.bin VBR.bin)
