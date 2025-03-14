# Maldev Notes


## Windows API

### Windows Data Types

Some of the common data types are listed below:

- `DWORD` - A 32-bit unsigned integer, on both 32-bit and 64-bit systems, used to represent values from 0 up to (2^32 - 1).

```c
DWORD dwVariable = 42;
```

- `size_t` - Used to represent the size of an object. It's a 32-bit unsigned integer on 32-bit systems representing values from 0 up to (2^32 - 1). On the other hand, it's a 64-bit unsigned integer on 64-bit systems representing values from 0 up to (2^64 - 1).

```c
SIZE_T sVariable = sizeof(int);
```

- `VOID` - Indicates the absence of a specific data type.

```c
void* pVariable = NULL; // This is the same as PVOID
```

- `PVOID` - A 32-bit or 4-byte pointer of any data type on 32-bit systems. Alternatively, a 64-bit or 8-byte pointer of any data type on 64-bit systems.

```c
PVOID pVariable = &SomeData;
```

- `HANDLE` - A value that specifies a particular object that the operating system is managing (e.g. file, process, thread).

```c
HANDLE hFile = CreateFile(...);
```

- `HMODULE` - A handle to a module. This is the base address of the module in memory. An example of a MODULE can be a DLL or EXE file.

```c
HMODULE hModule = GetModuleHandle(...);
```

- `LPCSTR/PCSTR` - A pointer to a constant null-terminated string of 8-bit Windows characters (ANSI). The "L" stands for "long" which is derived from the 16-bit Windows programming period, nowadays it doesn't affect the data type, but the naming convention still exists. The "C" stands for "constant" or read-only variable. Both these data types are equivalent to `const char*`.

```c
LPCSTR  lpcString   = "Hello, world!";
PCSTR   pcString    = "Hello, world!";
```

- `LPSTR/PSTR` - The same as `LPCSTR` and `PCSTR`, the only difference is that `LPSTR` and `PSTR` do not point to a constant variable, and instead point to a readable and writable string. Both these data types are equivalent to `char*`.

```c
LPSTR   lpString    = "Hello, world!";
PSTR    pString     = "Hello, world!";
```

- `LPCWSTR\PCWSTR` - A pointer to a constant null-terminated string of 16-bit Windows Unicode characters (Unicode). Both these data types are equivalent to `const wchar*`.

```c
LPCWSTR     lpwcString  = L"Hello, world!";
PCWSTR      pcwString   = L"Hello, world!";
```

- `PWSTR\LPWSTR` - The same as `LPCWSTR` and `PCWSTR`, the only difference is that 'PWSTR' and 'LPWSTR' do not point to a constant variable, and instead point to a readable and writable string. Both these data types are equivalent to `wchar*`.

```c
LPWSTR  lpwString   = L"Hello, world!";
PWSTR   pwString    = L"Hello, world!";
```

- `wchar_t` - The same as `wchar` which is used to represent wide characters.

```c
wchar_t     wChar           = L'A';
wchar_t*    wcString        = L"Hello, world!";
```

- `ULONG_PTR` - Represents an unsigned integer that is the same size as a pointer on the specified architecture, meaning on 32-bit systems a `ULONG_PTR` will be 32 bits in size, and on 64-bit systems, it will be 64 bits in size. Throughout this course, `ULONG_PTR` will be used in the manipulation of arithmetic expressions containing pointers (e.g. PVOID). Before executing any arithmetic operation, a pointer will be subjected to type-casting to `ULONG_PTR`. This approach is used to avoid direct manipulation of pointers which can lead to compilation errors.

```c
PVOID Pointer = malloc(100);
// Pointer = Pointer + 10; // not allowed
Pointer = (ULONG_PTR)Pointer + 10; // allowed
```


### Data Types Pointers

The Windows API allows a developer to declare a data type directly or a pointer to the data type. This is reflected in the data type names where the data types that start with "P" represent pointers to the actual data type while the ones that don't start with "P" represent the actual data type itself.

This will become useful later when working with Windows APIs that have parameters that are pointers to a data type. The examples below show how the "P" data type relates to its non-pointer equivalent.

- `PHANDLE` is the same as `HANDLE*`.
    
- `PSIZE_T` is the same as `SIZE_T*`.
    
- `PDWORD` is the same as `DWORD*`.
    

### ANSI & Unicode Functions

The majority of Windows API functions have two versions ending with either "A" or with "W". For example, there is [CreateFileA](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea) and [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew). The functions ending with "A" are meant to indicate "ANSI" whereas the functions ending with "W" represent Unicode or "Wide".

The main difference to keep in mind is that the ANSI functions will take in ANSI data types as parameters, where applicable, whereas the Unicode functions will take in Unicode data types. For example, the first parameter for `CreateFileA` is an `LPCSTR`, which is a pointer to a constant null-terminated string of **8-bit** Windows ANSI characters. On the other hand, the first parameter for `CreateFileW` is `LPCWSTR`, a pointer to a constant null-terminated string of **16-bit** Unicode characters.

Furthermore, the number of required bytes will differ depending on which version is used.

`char str1[] = "maldev";` // 7 bytes (maldev + [null byte](https://www.tutorialandexample.com/null-character-in-c)).

`wchar str2[] = L"maldev";` // 14 bytes, each character is 2 bytes (The null byte is also 2 bytes)

### In and Out Parameters

Windows APIs have [in](https://learn.microsoft.com/en-us/windows/win32/midl/in) and [out](https://learn.microsoft.com/en-us/windows/win32/midl/out-idl) parameters. An `IN` parameter is a parameter that is passed into a function and is used for input. Whereas an `OUT` parameter is a parameter used to return a value back to the caller of the function. Output parameters are often passed in by reference through pointers.

For example, the code snippet below shows a function `HackTheWorld` which takes in an integer pointer and sets the value to `123`. This is considered an out parameter since the parameter is returning a value.

```c
BOOL HackTheWorld(OUT int* num){

    // Setting the value of num to 123
    *num = 123;
    
    // Returning a boolean value
    return TRUE;
}

int main(){
    int a = 0;

    // 'HackTheWorld' will return true
    // 'a' will contain the value 123
    HackTheWorld(&a);
}
```
### API Example

When working with the WinAPI we should consult documentation often this will tell you all the attributes/elements that are in a struct
For example the CreateFileW API function [documentation](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew) we see the following

```
HANDLE CreateFileW(
  [in]           LPCWSTR               lpFileName,
  [in]           DWORD                 dwDesiredAccess,
  [in]           DWORD                 dwShareMode,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  [in]           DWORD                 dwCreationDisposition,
  [in]           DWORD                 dwFlagsAndAttributes,
  [in, optional] HANDLE                hTemplateFile
);
```


Using the api function and explaining each parameter passed to the function below:

``` cpp
int main() {
	//invalid value just for variable init
	HANDLE hFile = INVALID_HANDLE_VALUE;
	//full path of file to create
	LPCWSTR filePath = L"C:\\Users\\h8han\\Desktop\\maldev.txt";
	//CreateFileW takes 7 values to build out the struct
	//the path we defined above
	//desired access generic all in this case
	//sharemode 0 Prevents subsequent open operations on a file or device if they request delete, read, or write access.
	//security attributes are optional so we leave null 
	//creation disposition An action to take on a file or device that exists or does not exist
	//file attribute normal The file does not have other attributes set. This attribute is valid only if used alone.
	//template file is optional so we leave null
	hFile = CreateFileW(filePath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);



	//if failed returns invalid handle value
	//GetLastError retrieves error code of the previously executed winapi function.
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("[-] CreateFileW api function failed with error : %d\n", GetLastError());
		return -1;
	}

}

```

Some api functions will not have documentation because windows isn't an open system after all there
are some alternatives that others have created for these unknown functions found through reverse engineering

[Process Hacker's Header Files](https://github.com/winsiderss/systeminformer/tree/master/phnt/include)

[undocumented.ntinternals.net](https://web.archive.org/web/20230401045934/http://undocumented.ntinternals.net/) - Some structures may be outdated

[ReactOS's Documentation](https://doxygen.reactos.org/globals_type.html)

[Vergilius Project](https://www.vergiliusproject.com/) - Although mainly for Windows kernel structures, it remains a valuable resource.

