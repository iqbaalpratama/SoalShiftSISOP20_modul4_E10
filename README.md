## SoalShiftSISOP20_modul4_E10
**Soal**
Di suatu perusahaan, terdapat pekerja baru yang super jenius, ia bernama jasir. Jasir baru bekerja selama seminggu di perusahan itu, dalam waktu seminggu tersebut ia selalu terhantui oleh ketidak amanan dan ketidak efisienan file system yang digunakan perusahaan tersebut. Sehingga ia merancang sebuah file system yang sangat aman dan efisien. Pada segi keamanan, Jasir telah menemukan 2 buah metode enkripsi file. Pada mode enkripsi pertama, nama file-file pada direktori terenkripsi akan dienkripsi menggunakan metode substitusi. Sedangkan pada metode enkripsi yang kedua, file-file pada direktori terenkripsi akan di-split menjadi file-file kecil. Sehingga orang-orang yang tidak menggunakan filesystem rancangannya akan kebingungan saat mengakses direktori terenkripsi tersebut. Untuk segi efisiensi, dikarenakan pada perusahaan tersebut sering dilaksanakan sinkronisasi antara 2 direktori, maka jasir telah merumuskan sebuah metode agar filesystem-nya mampu mengsingkronkan kedua direktori tersebut secara otomatis. Agar integritas filesystem tersebut lebih terjamin, maka setiap command yang dilakukan akan dicatat kedalam sebuah file log.
(catatan: filesystem berfungsi normal layaknya linux pada umumnya)
(catatan: mount source (root) filesystem adalah direktori /home/[user]/Documents)
Berikut adalah detail filesystem rancangan jasir:

 **1.  Enkripsi versi 1:**
1.  Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
2.  Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
3.  Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.
4.  Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
5.  Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.  

```9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO```

Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10  
“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg

Note  : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.

6.  Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

**Jawaban :**
```c
char cipher[100]="9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

void encrypt(char* str)
{
	if(!strcmp(str,".") || !strcmp(str,"..")) return;
	int panjang = strlen(str);
    int start=0;
    for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
    for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/')
        {
            start = i;
            break;
        }
      
    }

	for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+10)%87];
				break;
			}
		}
	}
}


void decrypt(char* str)
{
	if(!strcmp(str,".") || !strcmp(str,"..")) return;
	int panjang = strlen(str);
    int start=0;
    for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
    for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/' || str[i + 1] == '\0')
        {
            start = i+1;
            break;
        }
      
    }
	for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+77)%87];
				break;
			}
		}
	}
}
```
Kode diatas adalah kode enkripsi dan dekripsi untuk versi 1.
```c
void encrypt(char* str)
{
	if(!strcmp(str,".") || !strcmp(str,"..")) return;
	int panjang = strlen(str);
    int start=0;
    for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
    for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/')
        {
            start = i;
            break;
        }
      
    }

	for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+10)%87];
				break;
			}
		}
	}
}
```
Untuk kode enkripsi `if(!strcmp(str,".") || !strcmp(str,"..")) return;` digunakan agar pada folder dengan nama `.` dan `..` tidak ikut terenkripsi
```c
for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
```
sedangkan kode diatas digunakan agar ekstensi tidak ikut terenkripsi
```c
for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/')
        {
            start = i;
            break;
        }
      
    }
```
Kode diatas digunakan untuk mencari awal mula perulangan untuk enkripsi dimana dimulai setelah tanda `/` pertama setelah kata `encv1`
```c
for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+10)%87];
				break;
			}
		}
	}
```
Dan kode diatas yaitu untuk memulai enkripsinya dimana setiap karakter pada nama folder akan digeser sebanyak 10 karakter pada kode cipher yang digunakan.
Sedangkan pada bagian dekripsi untuk versi 1 hampir sama dengan kode enkripsi
```c
void decrypt(char* str)
{
	if(!strcmp(str,".") || !strcmp(str,"..")) return;
	int panjang = strlen(str);
    int start=0;
    for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
    for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/' || str[i + 1] == '\0')
        {
            start = i+1;
            break;
        }
      
    }
	for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+77)%87];
				break;
			}
		}
	}
}
```
Untuk kode dekripsi `if(!strcmp(str,".") || !strcmp(str,"..")) return;` digunakan agar pada folder dengan nama `.` dan `..` tidak ikut terdekripsi
```c
for(int i=panjang;i>=0;i--)
	{
		if(str[i]=='.')
        {
            panjang=i;
            break;
        }
        
	}
```
sedangkan kode diatas digunakan agar ekstensi tidak ikut terdekripsi.
Kode berikut digunakan agar pada bagian nama folder yang memiliki `encv1_` tidak terdekripsi
```c
 for (int i = 1; i < panjang; i++)
    {
        if (str[i] == '/' || str[i + 1] == '\0')
        {
            start = i+1;
            break;
        }
      
    }
```
```c
for(int i=start;i<panjang;i++)
	{
		for(int j=0;j<87;j++)
		{
			if(str[i]==cipher[j])
			{
				str[i] = cipher[(j+77)%87];
				break;
			}
		}
	}
```
Dan kode diatas yaitu untuk memulai dekripsinya dimana setiap karakter pada nama folder akan digeser sebanyak (87-10) atau 77 karakter pada kode cipher yang digunakan

Untuk penggunaannya yaitu pada getattr maka path jika mengandung string `encv1` maka akan didecrypt seperti berikut:
```c
static int do_getattr(const char *path, struct stat *st){
    printf("getattr %s\n",path);
    
    int res;
    char fpath[1000];
    char paths[100];
    char name[1000];
    strcpy(paths, path);
    strcpy(name,path);
    if(strstr(path, "encv1_")!=NULL)
    {
        decrypt(name);
    }
    else
    {
        strcpy(name,path);
    }
    printf("%s\n", name);
    if(print_info_command("GETATTR", paths)){
        sprintf(fpath, "%s/%s", dirpath, name);
        res = lstat(fpath, st);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan getattr sukses\n");
        }
    }
    return 0;
}
```
Sedangkan pada readdir, untuk setiap de->d_name dari folder yang mengandung string `encv1` maka harus dienkripsi
```c
static int do_readdir(const char *path, void *buf, fuse_fill_dir_t filler, 
    off_t offset, struct fuse_file_info *fi){
    printf("readdir\n");
    char fpath[1000];
     char name[1000];
     int mode=0;
    if(strstr(path, "encv1_")!=NULL)
    {
        mode=1;
    }
    if(strcmp(path, "/") == 0){
        path = dirpath;
        sprintf(fpath, "%s", path);
    }else{
        
        sprintf(fpath, "%s%s", dirpath, path);
    }

    int res = 0;

    DIR *dp;
    struct dirent *de;
    char paths[100];
   
    strcpy(paths, path);
    // (void) offset;
    // (void) fi;
    if(print_info_command("READDIR", paths)){
        dp = opendir(fpath);
        if(dp == NULL){
            return -errno;
        }else{
            while ((de = readdir(dp)) != NULL) {
            struct stat st;
            memset(&st, 0, sizeof(st));
            st.st_ino = de->d_ino;
            st.st_mode = de->d_type << 12;
              char temp[1000];
            strcpy(temp, de->d_name);
            if(mode==1)
            {
                encrypt(temp);
            }
            res = (filler(buf, temp, &st, 0));
                if(res!=0) {
                    break;
                }
            }
            printf("penulisan dan readdir sukses\n");
            closedir(dp);
        }
    }
    return 0;
}
```
Untuk fungsi fungsi lain yaitu akan didecrypt pathnya jika mengandung string `encv1` 

**2. Enkripsi versi 2:**
1.  Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
2.  Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
3.  Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.
4.  Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
5.  Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file kecil yakni: File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004.
6.  Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif).

**Jawaban**

**3. Sinkronisasi direktori otomatis:**
Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:

1.  Kedua directory memiliki parent directory yang sama.    
2.  Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:
3.  Nama dari setiap berkas di dalamnya sama.
4.  Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.
5.  Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.
6.  Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory.
    
Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.

Implementasi dilarang menggunakan symbolic links dan thread.

 **4. Log system:**
    
1.  Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
2.  Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
3.  Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
4.  Sisanya, akan dicatat dengan level INFO.
5.  Format untuk logging yaitu:

<kbd>    
[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]<br>
LEVEL : Level logging<br>
yy : Tahun dua digit<br>
mm : Bulan dua digit<br>
dd : Hari dua digit<br>
HH : Jam dua digit<br>
MM : Menit dua digit<br>
SS : Detik dua digit<br>
CMD : System call yang terpanggil<br>
DESC : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)
</kbd>

Contoh format logging nantinya seperti:
<kbd>
INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg
</kbd>

**Jawaban :**
Solusi kami memiliki konsep untuk setiap system call akan mentrigger fungsi-fungsinya masing-masing pada fuse. Sehingga kami tulis semua fungsi yang sekiranya berkaitan dengan system call dan melakukan pemanggilan fungsi custom untuk melakukan penulisan fle.
 
Pada bagian ini kita melakukan define path untuk source dari fuse dan path untuk menyimpn log system call.
```c
static const char *dirpath = "/home/syubban/Documents";
static const char *log_path = "/home/syubban/fs.log";
```
Kemudian 2 fungsi dibawah ini berfungsi sebagai penulis pada log system call 
```c
int print_info_command(char *command, char *desc){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
	sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
	tm.tm_year + 1900, tm.tm_mon + 1,
	tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, command, desc);
	printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}

int print_warning_command(char *command, char *desc){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char mains[1000];
	sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
	tm.tm_year + 1900, tm.tm_mon + 1,
	tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, command, desc);
	printf("%s", mains);
	FILE *foutput = fopen(log_path, "a+");
	fputs(mains, foutput);
	fclose(foutput);
	return 1;
}
```
time_t berfungsi untuk mengambil waktu pertama kali, kemudian kita casting ke localtime dan disimpan pada struct tm. Struct tm memiliki banyak properti untuk tahun, tanggal sampai detik. lalu sprintf berfungsi untuk mengisi char *mains dengan format sesuai permintaan soal. Penggunaan sprintf dikarenakan properti dari tm adalah integer dan kita ingin mengisikan pada char *, selain itu sprintf sangat memudahkan dalam menulis segala hal dalam char *. Setelah pengisian mains, mains kita append ke file log_path dimana kita buka dengan metode a+ (karena sudah dibuat pada init). Hasil mains langsung diletakkan pada file lalu kita close pointer file.
```c
static void *do_init(struct fuse_conn_info *conn){
	printf("enter => init");
	FILE *fpointer = fopen(log_path, "rb+");
	if(fpointer == NULL){
		fpointer = fopen(log_path, "wb");
		fclose(fpointer);
	}else{
		fclose(fpointer);
	}
	return 0;
}
```
Pada saat filesystem pertama kali di mount, filesystem akan memanggil fungsi do_init, sehingga ketika dimount juga kami buat file log. Untuk mount ke 2 dan selanjutnya supaya tidak merewrite log, maka ditambah kondisi ketika tidak ada kita buat dan ketika ada kita biarkan.
Berikut salah satu contoh untuk log tipe warning dan log tipe info (karena ada 15 fungsi lebih).

```c
//info level log
static  int  do_mkdir(const  char *path, mode_t  mode){
	printf("enter => mkdir\n");
	char  fpath[500] = "/home/syubban/Documents";
	strcat(fpath, path);
	char  paths[100];
	strcpy(paths, path);
	if(print_info_command("MKDIR", paths)){
		mkdir(fpath, 0777);
		printf("penulisan dan mkdir sukses\n");
	}
	return  0;
}
```
Ketika kita mengetikkan command mkdir, format pengetikannya sebagai berikut 
mkdir [nama folder]. Kami sudah menginisiasi full path pada fpath lalu kita concat dengan nama folder. Kemudian kita lakukan pengecekan untuk pemanggilan fungsi print_info_command lalu diberi value MKDIR dan input path (nama folder). Ketika sukses kita panggil system call sesuai fungsinya lalu print sesuatu. 
```c
static  int  do_rmdir(const  char *path){
	printf("Enter => rmdir\n");
	int res;
	char  fpath[500] = "/home/syubban/Documents";
	strcat(fpath, path);
	char  paths[100];
	strcpy(paths, path);
	if(print_warning_command("RMDIR", paths)){
		res = rmdir(fpath);
		if(res == -1){
			return -errno;
		}else{
			printf("penulisan dan rmdir sukses\n");
		}
	}
	return  0;
}
```
Ketika kita mengetikkan command rmdir, format pengetikannya sebagai berikut 
rmdir [nama folder]. Kami sudah menginisiasi full path pada fpath lalu kita concat dengan nama folder. Kemudian kita lakukan pengecekan untuk pemanggilan fungsi print_warning_command lalu diberi value RMDIR dan input path (nama folder). Ketika sukses kita panggil system call sesuai fungsinya lalu print sesuatu.


**KENDALA**:
Belum terlalu memahami fungsi-fungsi FUSE dan cara kerja dari setiap fungsi.  
