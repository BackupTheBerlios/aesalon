#add_custom_command(OUTPUT overload.o COMMAND /usr/bin/nasm -f elf64 overload.asm -o overload.o)# COMMAND gcc -fPIC -shared -O0 overload.o -o overload.so)
#add_custom_command(TARGET overload.so POST_BUILD COMMAND /usr/bin/nasm -f elf64 overload.asm -o overload.o COMMAND gcc -fPIC -shared -O0 overload.o -o overload.so)
#add_custom_command(OUTPUT overload.o COMMAND echo "hello" >> overload.o DEPENDS overload.asm WORKING_DIRECTORY src/monitor/overload)
#add_custom_command(OUTPUT overload.so COMMAND cp overload.o overload.so DEPENDS overload.o WORKING_DIRECTORY src/monitor/overload)
#add_custom_target(Overload /usr/bin/nasm -f elf64 overload.asm -o overload.o COMMAND gcc -shared overload.o -o overload.so)
if(DEVELOPMENT_BUILD)
    add_custom_command(OUTPUT aesalon_overload.so COMMAND /usr/bin/gcc -fPIC -g -shared overload.c -o aesalon_overload.so -ldl -masm=intel
        -DLIBC_PATH='"${LIBC_PATH}"'
        -DDEVELOPMENT_BUILD
        -DAESALON_PLATFORM=${AESALON_PLATFORM}
        -DAESALON_PLATFORM_x86=0
        -DAESALON_PLATFORM_x86_64=1
        DEPENDS overload.c)
else(DEVELOPMENT_BUILD)
    add_custom_command(OUTPUT aesalon_overload.so COMMAND /usr/bin/gcc -fPIC -shared overload.c -o aesalon_overload.so -ldl -masm=intel
        -DLIBC_PATH="${LIBC_PATH}" DEPENDS overload.c)
endif(DEVELOPMENT_BUILD)

add_custom_target("overload library" ALL DEPENDS aesalon_overload.so)
install(FILES aesalon_overload.so DESTINATION lib/)
