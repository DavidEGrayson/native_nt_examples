all:
	gcc launcher.c -o launcher.exe --std=gnu99 -municode -lntdll
	gcc test_native.c -o test_native.exe --std=gnu99 -lntdll -nostdlib \
	  -Wl,--subsystem,native -e NtProcessStartup
