arm-none-linux-gnueabi-gcc io_test.c -o io_test -lrt --static
adb push io_test /cache
