# C学习笔记

## 13.文件输入输出

### 1.fopen模式字符串

* r：读取
* w：截0写入不读
* a：追加写入不读
* r+：读取写入
* w+：截0读取写入
* a+：全部读取，追加写入
* b：二进制

fseek():origin

* SEEK_SET：起始位置
* SEEK_CUR：当前位置
* SEEK_END：文件结尾

### 2.IO函数

* `FILE * fopen ( const char * filename, const char * mode );`
* `int getc ( FILE * stream );`
* `int putc ( int character, FILE * stream );`
* `int fclose ( FILE * stream );`
* `int fprintf ( FILE * stream, const char * format, ... );`
* `int fscanf ( FILE * stream, const char * format, ... );`
* `char * fgets ( char * str, int num, FILE * stream );`
* `int fseek ( FILE * stream, long int offset, int origin );`
* `long int ftell ( FILE * stream );`
* `size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );`
* `size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );`