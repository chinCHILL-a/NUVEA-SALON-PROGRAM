#include <stdio.h>
#include <string.h>

// ====================================================
//                STRUKTUR DATA & GLOBAL 
struct Pelanggan {   
    char nama[50];
    char password[50];
    long totalBelanja; 
};
/* struct Pelanggan, digunakan untuk menyimpan data pelanggan dengan berbagai tipe yaitu nama, pass akun & total belanja yang kemudian dipake
di proses registrasi, login & hampir di semua module lain termasuk ke penyimpanan fie data customer */

struct Booking {
    char namaLayanan[100];
    int harga;
};
/* struct Booking ini digunakan saat customer ingin membooking sebuah layanan oleh karena itu berisi apa nama layanan yang dipilih customer
dan harga dari layanan yang dipilih */

struct Pelanggan daftarUser[100]; 
int jumlahUser = 0;
int userSekarang = -1; 
/*ini berlaku sebagai wadah (variabel array) atau buku catetan untuk menyimpan hasil dari struct pelanggan yang nantinya 
akan dialirkan ke file txt*/

struct Booking daftarBelanja[50]; //ini berlaku sebagai wadah (variabel array) atau buku catetan untuk menyimpan hasil dari struct booking yang nantinya akan dialirkan ke file txt
int jumlahBelanja = 0;
/* PENJELASAN : ketika customer memilih layanan, layanan tersebut akan masuk ke struct booking dan ditaruh ke daftar belanja.
Saat customer ingin membayar program akan mengecek apa saja isi didalam daftarBelanja untuk menghitung total harganya. 
Saat transaksi sudah dilakukan jumlah belanja dalam struct daftarBelanja akan diinilialisasi menjadi 0 kembali 
*/

// ====================================================
//                   MODUL UTILITY
void bersihinLayar() {             // untuk menghapus sisa command yang sempat dijalankan, jadi tetap rapih
    printf("\033[2J\033[H");
}

void tampilkanHeader() {                        //function header untuk ditampilkan saat registrasi, login, riwayat dll
    printf("----------------------------------------------------\n");
    printf("             NUVEA LUXURY SALON & SPA              \n");
    printf("          Where Elegance Meets Relaxation          \n");
    printf("----------------------------------------------------\n");
}

void tekanEnter() {                           //function untuk menampilan command untuk cust agar tetao mengklik enter untuk melanjutkan program  
    printf("\n[ Tekan Enter untuk lanjut ]"); 
    getchar(); 
    getchar(); 
}

void salinKata(char tujuan[], char asal[]) {
    strcpy(tujuan, asal);
}

int bandingKata(char kata1[], char kata2[]) {
    return strcmp(kata1, kata2) == 0;
}

//             MODUL MEMBERSHIP CARD
// ====================================================
void tampilkanMembershipCard() {                         //module ini digunakan untuk menampilkan membership card yang didalammnya sudah 
    long spend = daftarUser[userSekarang].totalBelanja;  //termasuk logika level nya dan tulisan benefit apa yang akan diterima custome
    char status[20];
    char nextLevel[20];
    long sisa = 0;
    
    // Logika Level & Sisa
    if (spend >= 1000000) {
        salinKata(status, "PLATINUM");
        salinKata(nextLevel, "MAX LEVEL");
    } else if (spend >= 500000) {
        salinKata(status, "GOLD");
        salinKata(nextLevel, "PLATINUM");
        sisa = 1000000 - spend;
    } else if (spend >= 250000) {
        salinKata(status, "SILVER");
        salinKata(nextLevel, "GOLD");
        sisa = 500000 - spend;
    } else {
        salinKata(status, "BASIC");
        salinKata(nextLevel, "SILVER");
        sisa = 250000 - spend;
    }

    bersihinLayar();
    tampilkanHeader();

    printf("\n====================================================\n");
    printf("||                                                ||\n");
    printf("||                MEMBERSHIP CARD                 ||\n");
    printf("||                                                ||\n");
    printf("||  Username     : %-30s ||\n", daftarUser[userSekarang].nama);
    printf("||  Level        : %-30s ||\n", status);
    printf("||  Total Spend  : Rp %-27ld ||\n", spend);
    if (sisa > 0) {
        printf("||  Next Level   : %-30s ||\n", nextLevel);
        printf("||  Need More    : Rp %-27ld ||\n", sisa);
    } else {
        printf("||  Status       : TOP LEVEL CUSTOMER             ||\n");
        printf("||                                                ||\n");
    }
    printf("||                                                ||\n");
    printf("====================================================\n");

    
    printf("\n--- YOUR BENEFITS ---\n");
    if (bandingKata(status, "PLATINUM")) {
        printf("- Diskon Otomatis 15%% Setiap Transaksi\n");
        printf("- Akses VIP Lounge & Free Drink\n");
    } else if (bandingKata(status, "GOLD")) {
        printf("- Diskon Otomatis 10%% Setiap Transaksi\n");
        printf("- Prioritas Antrian \n");
    } else if (bandingKata(status, "SILVER")) {
        printf("- Diskon Otomatis 5% Setiap Transaksi\n");
    } else {
        printf("- Kumpulkan transaksi Rp 250k untuk diskon Silver!\n");
    }
    
    tekanEnter();
}

// MODUL FILE & DATA -> ini merupakan module untuk menyimpan data customer agar tidak hilang walau sudah di close
// ====================================================
void simpanDataUser() {                         
    FILE *f = fopen("customer.txt", "w");
    int i, j;
    if (f == NULL) return;

    // didalam filenya sudah termasuk sorting untuk mengurutkan customer dari yang paling tinggi spend total belanjanya
    for (i = 0; i < jumlahUser - 1; i++) {
        for (j = 0; j < jumlahUser - i - 1; j++) {
            if (daftarUser[j].totalBelanja < daftarUser[j + 1].totalBelanja) {
                struct Pelanggan temp = daftarUser[j];
                daftarUser[j] = daftarUser[j + 1];
                daftarUser[j + 1] = temp;
            }
        }
    }

    fprintf(f, "DATA CUSTOMER NUVEA LUXURY SALON & SPA\n");
    fprintf(f, "------------------------------------------------------------\n");
    fprintf(f, "NO | USERNAME   | PASS      | SPEND        | STATUS\n");
    fprintf(f, "------------------------------------------------------------\n");
    
    for (i = 0; i < jumlahUser; i++) {
        char status[20];
        long b = daftarUser[i].totalBelanja;
        if (b >= 1000000) salinKata(status, "PLATINUM");
        else if (b >= 500000) salinKata(status, "GOLD");
        else if (b >= 250000) salinKata(status, "SILVER");
        else salinKata(status, "BASIC");
        
        fprintf(f, "%d | %-10s | %-9s | %-12ld | %s\n", 
                (i + 1), daftarUser[i].nama, daftarUser[i].password, b, status);
    }
    fclose(f);
}

void bacaDataUser() {                     //module / function untuk membaca data customer seperti nama, password & jumlah spend uang
    FILE *f = fopen("customer.txt", "r");
    char header[255];
    int no, i;
    char n[50], p[50], s[20];
    long uang;

    jumlahUser = 0;
    if (f == NULL) return;
    for(i = 0; i < 4; i++) fgets(header, sizeof(header), f);
    while (fscanf(f, "%d | %s | %s | %ld | %s\n", &no, n, p, &uang, s) != EOF) {
        salinKata(daftarUser[jumlahUser].nama, n);
        salinKata(daftarUser[jumlahUser].password, p);
        daftarUser[jumlahUser].totalBelanja = uang;
        jumlahUser++;
    }
    fclose(f);
}

void catatKeRiwayat(char nama[], int total, char metode[]) {     //function untuk menambahkan informasi transaksi ke file riwayat transaksi semua customer
    FILE *f = fopen("riwayat.txt", "a");
    if (f == NULL) return;
    fprintf(f, "USER:%s | TOTAL:Rp%d | METODE:%s\n", nama, total, metode);
    fclose(f);
}

void lihatRiwayat() {
    FILE *f = fopen("riwayat.txt", "r");                        //function untuk membaca informasi transaksi ke file riwayat transaksi semua customer
    char baris[255];
    char cariUser[60];
    
    bersihinLayar();
    tampilkanHeader();
    sprintf(cariUser, "USER:%s", daftarUser[userSekarang].nama);

    if (f == NULL) {
        printf("\nBelum ada riwayat transaksi.\n");
    } else {
        printf("\n--- RIWAYAT TRANSAKSI ANDA (%s) ---\n", daftarUser[userSekarang].nama);
        int ditemukan = 0;
        while (fgets(baris, sizeof(baris), f)) {
            if (strstr(baris, cariUser) != NULL) { 
                printf("- %s", baris);
                ditemukan = 1;
            }
        }
        if (!ditemukan) printf("Anda belum pernah melakukan transaksi.\n");
        fclose(f);
    }
    tekanEnter();
}

// ====================================================
//                  MODUL TRANSAKSI
void tampilkanStruk(int totalAsli, int diskon, int totalAkhir, char metode[]) {   //module untuk transaksi yang akan muncul setelah user melakukan transaksi
    int i;                                       
    printf("\n\n");  
    printf("   ------------------------------------------\n");
    printf("             NUVEA LUXURY STRUK              \n");
    printf("   ------------------------------------------\n");
    printf("    Customer : %-25s\n", daftarUser[userSekarang].nama);
    printf("    Metode   : %-25s\n", metode);
    printf("   ------------------------------------------\n");
    for (i = 0; i < jumlahBelanja; i++) {
        printf("    %-20s  Rp %d\n", daftarBelanja[i].namaLayanan, daftarBelanja[i].harga);
    }
    printf("   ------------------------------------------\n");
    printf("    Total Awal    : Rp %d\n", totalAsli);
    printf("    Diskon Member : Rp %d\n", diskon);
    printf("    TOTAL BAYAR   : Rp %-10d\n", totalAkhir);
    printf("   ------------------------------------------\n");
    printf("         Where Elegance Meets Relaxation     \n");
    printf("   ------------------------------------------\n");
}

void menuBooking() {
    int pilih, subPilih, konfirmasi, i;
    int loopBooking = 1;

    while (loopBooking) {
        bersihinLayar();
        tampilkanHeader();
        printf("\n--- CATEGORY MENU ---\n");
        printf("1. Salon & Hair Care\n");
        printf("2. Body Spa Treatments\n");
        printf("3. Lihat Keranjang & Checkout (%d Layanan)\n", jumlahBelanja);
        printf("4. Kembali ke Menu Utama\n");
        printf("\nPilih Kategori: ");
        scanf("%d", &pilih);

        if (pilih == 1) {
            printf("\n--- SALON & HAIR CARE ---\n");
            printf("1. Haircut (50k)\n2. Blow Dry (35k)\n3. Hair Wash (40k)\n4. Hair Spa (120k)\n");
            printf("Pilih Layanan: "); scanf("%d", &subPilih);
            if (subPilih == 1) { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Haircut"); daftarBelanja[jumlahBelanja].harga = 50000; }
            else if (subPilih == 2) { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Blow Dry"); daftarBelanja[jumlahBelanja].harga = 35000; }
            else if (subPilih == 3) { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Hair Wash"); daftarBelanja[jumlahBelanja].harga = 40000; }
            else { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Hair Spa"); daftarBelanja[jumlahBelanja].harga = 120000; }
            jumlahBelanja++;
            printf("\nLayanan masuk keranjang!"); tekanEnter();
        } 
        else if (pilih == 2) {
            printf("\n--- BODY SPA TREATMENTS ---\n");
            printf("1. Body Massage (150k)\n2. Body Scrub (130k)\n3. Signature Spa (250k)\n");
            printf("Pilih Layanan: "); scanf("%d", &subPilih);
            if (subPilih == 1) { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Body Massage"); daftarBelanja[jumlahBelanja].harga = 150000; }
            else if (subPilih == 2) { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Body Scrub"); daftarBelanja[jumlahBelanja].harga = 130000; }
            else { salinKata(daftarBelanja[jumlahBelanja].namaLayanan, "Signature Spa"); daftarBelanja[jumlahBelanja].harga = 250000; }
            jumlahBelanja++;
            printf("\nLayanan masuk keranjang!"); tekanEnter();
        }
        else if (pilih == 3) {
            if (jumlahBelanja == 0) {
                printf("\nKeranjang masih kosong!\n"); tekanEnter();
            } else {
                int totalAsli = 0, diskon = 0, totalAkhir = 0;
                long spend = daftarUser[userSekarang].totalBelanja;
                for (i = 0; i < jumlahBelanja; i++) totalAsli += daftarBelanja[i].harga;
                
                // Diskon Member
                if (spend >= 1000000) diskon = totalAsli * 0.15;
                else if (spend >= 500000) diskon = totalAsli * 0.10;
                else if (spend >= 250000) diskon = totalAsli * 0.05;
                totalAkhir = totalAsli - diskon;

                printf("\n--- KERANJANG ANDA ---\n");
                printf("Total Awal: Rp %d\n", totalAsli);
                printf("Diskon    : Rp %d\n", diskon);
                printf("Total Akhir: Rp %d\n", totalAkhir);
                printf("\nKonfirmasi Bayar? (1. Ya / 2. Nanti): "); scanf("%d", &konfirmasi);
                
                if (konfirmasi == 1) {
                    char m[30];
                    printf("Metode (1. CASH / 2. QRIS / 3. TRANSFER BANK): "); scanf("%d", &subPilih);
                    if (subPilih == 1) salinKata(m, "CASH");
                    else if (subPilih == 2) salinKata(m, "QRIS");
                    else salinKata(m, "TRANSFER BANK");
                    
                    daftarUser[userSekarang].totalBelanja += totalAkhir;
                    catatKeRiwayat(daftarUser[userSekarang].nama, totalAkhir, m);
                    simpanDataUser(); 
                    tampilkanStruk(totalAsli, diskon, totalAkhir, m);
                    jumlahBelanja = 0; 
                    tekanEnter();
                    loopBooking = 0; 
                }
            }
        }
        else {
            int yakin;
            printf("\nYakin ingin keluar? Keranjang akan dikosongkan (1. Ya / 2. Tidak): ");
            scanf("%d", &yakin);
            if(yakin == 1) {
                jumlahBelanja = 0;
                loopBooking = 0;
            }
        }
    }
}

// ====================================================
//                   MAIN PROGRAM -> apa yang akan dilihat user pertama kali saat me run programnya
int main() {
    int menuUtama, menuUser, i;
    char n[50], p[50];
    bacaDataUser();

    while (1) {
        bersihinLayar();
        tampilkanHeader();
        printf(" 1. Register Baru\n");
        printf(" 2. Login Member\n");
        printf(" 3. Keluar\n");
        printf("\nPilih Menu: ");
        scanf("%d", &menuUtama);

        if (menuUtama == 1) {
            printf("\n--- REGISTRASI ---\n");
            printf("Username : "); scanf("%s", n);
            printf("Password : "); scanf("%s", p);
            salinKata(daftarUser[jumlahUser].nama, n);
            salinKata(daftarUser[jumlahUser].password, p);
            daftarUser[jumlahUser].totalBelanja = 0;
            jumlahUser++;
            simpanDataUser(); 
            printf("\nRegistrasi Sukses!\n"); tekanEnter();
        } 
        else if (menuUtama == 2) {
            printf("\n--- LOGIN ---\n");
            printf("Username : "); scanf("%s", n);
            printf("Password : "); scanf("%s", p);
            userSekarang = -1;
            for (i = 0; i < jumlahUser; i++) {
                if (bandingKata(daftarUser[i].nama, n) && bandingKata(daftarUser[i].password, p)) {
                    userSekarang = i;
                    break;
                }
            }
            if (userSekarang != -1) {
                while (1) {
                    bersihinLayar();
                    tampilkanHeader();
                    printf("\nSelamat Datang, %s!\n", daftarUser[userSekarang].nama);
                    printf("1. Booking Layanan\n");
                    printf("2. Lihat Membership Card & Benefit\n");
                    printf("3. Lihat Riwayat Belanja Saya\n");
                    printf("4. Logout\n");
                    printf("\nPilih Menu: ");
                    scanf("%d", &menuUser);
                    if (menuUser == 1) menuBooking();
                    else if (menuUser == 2) tampilkanMembershipCard();
                    else if (menuUser == 3) lihatRiwayat();
                    else break; 
                }
            } else {
                printf("\nUser tidak ditemukan!\n"); tekanEnter();
            }
        } 
        else break;
    }
    return 0;
}
