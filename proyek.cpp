#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

// File untuk menyimpan jumlah ID Driver
std::string COUNTER_ID_DRIVER = "ambulance_counter.txt";
// File untuk menyimpan jumlah ID Pasien
std::string COUNTER_ID_PASIEN = "pasien_counter.txt";
// File untuk menyimpan jumlah ID Rumah Sakit
std::string COUNTER_ID_RS = "RS_counter.txt";

// File untuk menyimpan data Ambulance
std::string DATA_AMBULANCE = "data_ambulance.txt";
// File untuk menyimpan data Pasien
std::string DATA_PASIEN = "data_pasien.txt";
// File untuk menyimpan data Ambulance
std::string DATA_RS = "data_RS.txt";
// File untuk menyimpan data Transaksi
std::string DATA_TRANSAKSI = "data_transaksi.txt";

// FUNGSI AUTO_INCREMENT
// Baca counter dari file
int loadCounter(std::string fileName)
{
    std::ifstream file(fileName);
    int counter = 0;
    if (file)
    {
        file >> counter;
        file.close();
    }
    return counter;
}

// Simpan counter ke file
void saveCounter(int counter, std::string fileName)
{
    std::ofstream file(fileName);
    if (file)
    {
        file << counter;
        file.close();
    }
}
// AKHIR FUNGSI AUTO_INCREMENT

// ========= AWAL STRUCT ==========

// Struktur Ambulance
struct Ambulance
{
    std::string id;
    std::string driverName;
    std::string status; // "Ready" / "Maintenance"
    double locationX;
    double locationY;
    Ambulance *next; // linked list
};

typedef Ambulance *ptrAmbulance;

// Struktur Pasien
struct Pasien
{
    std::string id;
    std::string name;
    std::string location;
    int priority;
    double locationX;
    double locationY;
    Pasien *next; // linked list
};

typedef Pasien *ptrPasien;

// Struktur Rumah Sakit
struct RumahSakit
{
    std::string id;
    std::string name;
    double locationX;
    double locationY;
    RumahSakit *next;
};

typedef RumahSakit *ptrRumahSakit;

// Struktur Transaksi
struct Transaksi
{
    std::string pasienId;
    std::string pasienName;
    std::string ambulanceDriver;
    std::string action;   // "jemput" atau "antar"
    std::string keLokasi; // lokasi tujuan
    Transaksi *next;      // linked list
};

typedef Transaksi *ptrTransaksi;

// Queue untuk antrean pasien
struct Queue
{
    Pasien *head;
    Pasien *tail;
};

// Stack untuk histori
struct Stack
{
    Transaksi *Top;
};

// ========= AKKHIR STRUCT ==========

// fungsi untuk meng-generate ID otomatis
std::string generateDriverId()
{
    int counter = loadCounter(COUNTER_ID_DRIVER); // Baca counter saat pertama kali dipanggil
    counter++;
    saveCounter(counter, COUNTER_ID_DRIVER); // Simpan perubahan ke file
    std::stringstream ss;
    ss << "DR" << std::setw(3) << std::setfill('0') << counter;
    return ss.str();
}

// fungsi untuk meng-generate ID otomatis
std::string generatePasienId()
{
    int counter = loadCounter(COUNTER_ID_PASIEN); // Baca counter saat pertama kali dipanggil
    counter++;
    saveCounter(counter, COUNTER_ID_PASIEN); // Simpan perubahan ke file
    std::stringstream ss;
    ss << "PS" << std::setw(3) << std::setfill('0') << counter;
    return ss.str();
}

// fungsi untuk meng-generate ID otomatis
std::string generateRSId()
{
    int counter = loadCounter(COUNTER_ID_RS); // Baca counter saat pertama kali dipanggil
    counter++;
    saveCounter(counter, COUNTER_ID_RS); // Simpan perubahan ke file
    std::stringstream ss;
    ss << "RS" << std::setw(3) << std::setfill('0') << counter;
    return ss.str();
}

// ========= AWAL AMBULANCE ==========

// Fungsi untuk menyimpan semua ambulance ke file
void saveAllAmbulancesToFile(ptrAmbulance head)
{
    std::ofstream file(DATA_AMBULANCE, std::ios::app);
    if (!file)
    {
        std::cout << "Gagal membuka file untuk penyimpanan!\n";
    }
    else
    {

        ptrAmbulance current = head;
        while (current != nullptr)
        {
            file << current->id << ","
                 << current->driverName << ","
                 << current->status << ","
                 << current->locationX << ","
                 << current->locationY << "\n";
            current = current->next;
        }
        file.close();
    }
}

// Fungsi untuk memuat / load semua data ambulance dari file ke linked list
void loadAllAmbulancesFromFile(ptrAmbulance &head)
{
    std::ifstream file(DATA_AMBULANCE);
    if (!file)
    {
        std::cout << "File data ambulance belum ada atau kosong.\n";
    }
    else
    {
        // Kosongkan linked list yang ada agar tidak memory leak
        while (head != nullptr)
        {
            ptrAmbulance temp = head;
            head = head->next;
            delete temp;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string token;
            Ambulance amb;

            std::getline(ss, token, ',');
            amb.id = token;

            std::getline(ss, token, ',');
            amb.driverName = token;

            std::getline(ss, token, ',');
            amb.status = token;

            std::getline(ss, token, ',');
            amb.locationX = std::stod(token);

            std::getline(ss, token, ',');
            amb.locationY = std::stod(token);

            // Tambahkan ke linked list
            ptrAmbulance pNew = new Ambulance{amb.id, amb.driverName, amb.status,
                                              amb.locationX, amb.locationY, nullptr};

            if (head == nullptr)
            {
                head = pNew;
            }
            else
            {
                ptrAmbulance temp = head;
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = pNew;
            }
        }
        file.close();
    }
}

// Fungsi buat list
void createListAmbulance(ptrAmbulance &head)
{
    head = nullptr;
}

// Fungsi menambahkan ambulance
void tambahAmbulance(ptrAmbulance &head, std::string driver, std::string status, double x, double y)
{
    std::string driverId = generateDriverId();
    ptrAmbulance pNew = new Ambulance{driverId, driver, status, x, y, nullptr};

    // Simpan ke file
    saveAllAmbulancesToFile(pNew);

    std::cout << "Ambulance ID " << driverId << " ditambahkan.\n";
}

// Menampilkan ambulance
void tampilkanAmbulance(ptrAmbulance head)
{
    loadAllAmbulancesFromFile(head); // Muat data dari file
    if (head == nullptr)
    {
        std::cout << "Daftar ambulance kosong.\n";
        return;
    }

    // Header tabel
    std::cout << "+--------+--------------+-------------+----------------+\n";
    std::cout << "|   ID   | Nama Driver  |   Status    |     Lokasi     |\n";
    std::cout << "+--------+--------------+-------------+----------------+\n";

    ptrAmbulance temp = head;
    while (temp != nullptr)
    {
        // print setiap baris data
        std::cout << "| " << std::setw(6) << temp->id << " | "
                  << std::setw(12) << std::left << temp->driverName << std::right << " | "
                  << std::setw(11) << std::left << temp->status << std::right << " | "
                  << "(" << std::setw(5) << temp->locationX << ", "
                  << std::setw(5) << std::left << temp->locationY << std::right << ") |\n";

        temp = temp->next;
    }

    // Footer tabel
    std::cout << "+--------+--------------+-------------+----------------+\n";
}
// =========== AKHIR AMBULANCE ===========

// =========== AWAL PASIEN ============
Queue antrianPasien = {nullptr, nullptr}; // Inisialisasi queue kosong

// Fungsi untuk menyimpan semua ambulance ke file
void saveAllPasienToFile(ptrPasien head)
{
    std::ofstream file(DATA_PASIEN, std::ios::app);
    if (!file)
    {
        std::cout << "Gagal membuka file untuk penyimpanan!\n";
    }
    else
    {
        ptrPasien current = head;
        while (current != nullptr)
        {
            file << current->id << ","
                 << current->name << ","
                 << current->location << ","
                 << current->priority << ","
                 << current->locationX << ","
                 << current->locationY << "\n";
            current = current->next;
        }
        file.close();
    }
}

// Fungsi untuk memuat / load semua data pasien dari file ke linked list
void loadAllPasienFromFile(ptrPasien &head)
{
    std::ifstream file(DATA_PASIEN);
    if (!file)
    {
        std::cout << "File data pasien belum ada atau kosong.\n";
    }
    else
    {
        // Kosongkan linked list yang ada agar tidak memory leak
        while (head != nullptr)
        {
            ptrPasien temp = head;
            head = head->next;
            delete temp;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string token;
            Pasien psn;

            std::getline(ss, token, ',');
            psn.id = token;

            std::getline(ss, token, ',');
            psn.name = token;

            std::getline(ss, token, ',');
            psn.location = token;

            std::getline(ss, token, ',');
            psn.priority = std::stoi(token);

            std::getline(ss, token, ',');
            psn.locationX = std::stod(token);

            std::getline(ss, token, ',');
            psn.locationY = std::stod(token);

            // Tambahkan ke linked list
            ptrPasien pNew = new Pasien{psn.id, psn.name, psn.location, psn.priority,
                                        psn.locationX, psn.locationY, nullptr};

            if (head == nullptr)
            {
                head = pNew;
            }
            else
            {
                ptrPasien temp = head;
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = pNew;
            }
        }
        file.close();
    }
}

void createListPasien(ptrPasien &head)
{
    head = nullptr;
}

// Menambahkan pasien
void tambahPasien(ptrPasien &head, std::string name, std::string location, int priority, double x, double y)
{
    std::string pasienId = generatePasienId();
    ptrPasien pNew = new Pasien{pasienId, name, location, priority, x, y, nullptr};

    // Simpan ke file
    saveAllPasienToFile(pNew);

    std::cout << "Pasien " << name << " ditambahkan.\n";
}

void tampilkanPasien(ptrPasien head)
{
    loadAllPasienFromFile(head);
    if (head == nullptr)
    {
        std::cout << "Daftar pasien kosong.\n";
    }
    else
    {
        // Header tabel
        std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";
        std::cout << "|   ID   |   Nama Pasien   |   Priority   | Tempat Kejadian |     Lokasi     |\n";
        std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";

        std::string ATS[4] = {"Red", "Yellow", "Green", "Black"};

        ptrPasien temp = head;
        while (temp != nullptr)
        {
            // print setiap baris data
            std::string triase = ATS[temp->priority - 1];

            std::cout << "| " << std::setw(6) << temp->id << " | "
                      << std::setw(15) << std::left << temp->name << std::right << " | "
                      << std::setw(12) << std::left << triase << std::right << " | "
                      << std::setw(15) << std::left << temp->location << std::right << " | "
                      << "(" << std::setw(5) << temp->locationX << ", "
                      << std::setw(5) << std::left << temp->locationY << std::right << ") |\n";
            temp = temp->next;
        }
        std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";
    }
}

void enqueuePasien(Queue &q, ptrPasien pasienBaru) {    
    if (q.head == nullptr && q.tail == nullptr) {
        q.head = pasienBaru;
        q.tail = pasienBaru;
    } else {
        ptrPasien current = q.head;
        ptrPasien prev = nullptr;
        
        while (current != nullptr && current->priority <= pasienBaru->priority) {
            prev = current;
            current = current->next;
        }
        
        if (prev == nullptr) {
            // Masukkan di depan (prioritas tertinggi)
            pasienBaru->next = q.head;
            q.head = pasienBaru;
        } else if (current == nullptr) {
            // Masukkan di belakang (prioritas terendah)
            q.tail->next = pasienBaru;
            q.tail = pasienBaru;
        } else {
            // Masukkan di tengah
            prev->next = pasienBaru;
            pasienBaru->next = current;
        }
    }
    std::cout << "Pasien " << pasienBaru->name << " dimasukkan ke antrian.\n";
}

void dequeuePasien(Queue &q) {
    if (q.head == nullptr && q.tail == nullptr) {
        std::cout << "Antrian kosong, tidak ada pasien yang bisa diproses.\n";
    } else {
        ptrPasien pasienKeluar = q.head;
        q.head = q.head->next;
        
        if (q.head == nullptr) {
            q.tail = nullptr;
        }
        
        std::cout << "Pasien " << pasienKeluar->name << " dikeluarkan dari antrian.\n";
        
        delete pasienKeluar;
    }
}

void tampilkanAntrian(Queue q) {
    if (q.head == nullptr) {
        std::cout << "Antrian pasien kosong.\n";
        return;
    }

    std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";
    std::cout << "|   ID   |   Nama Pasien   |   Priority   | Tempat Kejadian |     Lokasi     |\n";
    std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";

    std::string ATS[5] = {"Red", "Orange", "Green", "Blue", "White"};
    ptrPasien current = q.head;
    
    while (current != nullptr) {
        std::string triase = ATS[current->priority - 1];
        std::cout << "| " << std::setw(6) << current->id << " | "
                  << std::setw(15) << std::left << current->name << std::right << " | "
                  << std::setw(12) << std::left << triase << std::right << " | "
                  << std::setw(15) << std::left << current->location << std::right << " | "
                  << "(" << std::setw(5) << current->locationX << ", "
                  << std::setw(5) << std::left << current->locationY << std::right << ") |\n";
        current = current->next;
    }
    std::cout << "+--------+-----------------+--------------+-----------------+----------------+\n";
}

// ============== AKHIR PASIEN =============

// ============== RUMAH SAKIT ==============
// Fungsi untuk menyimpan semua RS ke file
void saveAllRumahSakitToFile(ptrRumahSakit head)
{
    std::ofstream file(DATA_RS, std::ios::app);
    if (!file)
    {
        std::cout << "Gagal membuka file untuk penyimpanan!\n";
    }
    else
    {

        ptrRumahSakit current = head;
        while (current != nullptr)
        {
            file << current->id << ","
                 << current->name << ","
                 << current->locationX << ","
                 << current->locationY << "\n";
            current = current->next;
        }
        file.close();
    }
}

// Fungsi untuk memuat / load semua data pasien dari file ke linked list
void loadAllRumahSakitFromFile(ptrRumahSakit &head)
{
    std::ifstream file(DATA_RS);
    if (!file)
    {
        std::cout << "File data Rumah Sakit belum ada atau kosong.\n";
    }
    else
    {
        // Kosongkan linked list yang ada agar tidak memory leak
        while (head != nullptr)
        {
            ptrRumahSakit temp = head;
            head = head->next;
            delete temp;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string token;
            RumahSakit rs;

            std::getline(ss, token, ',');
            rs.id = token;

            std::getline(ss, token, ',');
            rs.name = token;

            std::getline(ss, token, ',');
            rs.locationX = std::stod(token);

            std::getline(ss, token, ',');
            rs.locationY = std::stod(token);

            // Tambahkan ke linked list
            ptrRumahSakit pNew = new RumahSakit{rs.id, rs.name, rs.locationX, rs.locationY, nullptr};

            if (head == nullptr)
            {
                head = pNew;
            }
            else
            {
                ptrRumahSakit temp = head;
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = pNew;
            }
        }
        file.close();
    }
}

void tampilkanRumahSakit(ptrRumahSakit head) {
    loadAllRumahSakitFromFile(head);
    if (head == nullptr) {
        std::cout << "Daftar rumah sakit kosong.\n";
        return;
    }

    std::cout << "+--------+----------------------+----------------+\n";
    std::cout << "|   ID   |   Nama Rumah Sakit   |     Lokasi     |\n";
    std::cout << "+--------+----------------------+----------------+\n";

    ptrRumahSakit current = head;
    while (current != nullptr) {
        std::cout << "| " << std::setw(6) << current->id << " | "
                  << std::setw(20) << std::left << current->name << std::right << " | "
                  << "(" << std::setw(5) << current->locationX << ", "
                  << std::setw(5) << std::left << current->locationY << std::right << ") |\n";
        current = current->next;
    }
    std::cout << "+--------+----------------------+----------------+\n";
}

void tambahRumahSakit(ptrRumahSakit &head, std::string name, double x, double y) {
    std::string rsId = generateRSId();
    ptrRumahSakit pNew = new RumahSakit{rsId, name, x, y, nullptr};

    // Simpan ke file
    saveAllRumahSakitToFile(pNew);

    std::cout << "Rumah Sakit " << name << " ditambahkan dengan ID " << rsId << ".\n";
}

// ============== AKHIR RUMAH SAKIT ==============

// =========== AWAL TRANSAKSI ============

void saveAllTransaksiToFile(ptrTransaksi head) {
    std::ofstream file(DATA_TRANSAKSI, std::ios::app);
    if (!file)
    {
        std::cout << "Gagal membuka file untuk penyimpanan!\n";
    }
    else
    {
        ptrTransaksi current = head;
        while (current != nullptr) {
            file << current->pasienId << ","
                 << current->pasienName << ","
                 << current->ambulanceDriver << ","
                 << current->action << ","
                 << current->keLokasi << "\n";
            current = current->next;
        }
        file.close();
    }
}

void loadAllTransaksiFromFile(ptrTransaksi &head) {
    std::ifstream file(DATA_TRANSAKSI);
    if (!file) {
        std::cout << "File data pasien belum ada atau kosong.\n";
    } else {
        while (head != nullptr) {
            ptrTransaksi temp = head;
            head = head->next;
            delete temp;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            Transaksi trx;

            std::getline(ss, token, ',');
            trx.pasienId = token;

            std::getline(ss, token, ',');
            trx.pasienName = token;

            std::getline(ss, token, ',');
            trx.ambulanceDriver = token;

            std::getline(ss, token, ',');
            trx.action = token;

            std::getline(ss, token, ',');
            trx.keLokasi = token;

            ptrTransaksi pNew = new Transaksi{trx.pasienId, trx.pasienName, trx.ambulanceDriver,
                                              trx.action, trx.keLokasi, nullptr};
            
            if (head == nullptr) {
                head = pNew;
            } else {
                ptrTransaksi temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = pNew;
            }
        }
        file.close();
    }
}

void push(Stack &s, ptrTransaksi t) {
    t->next = s.Top;
    s.Top = t;
    saveAllTransaksiToFile(s.Top);
}

void pop(Stack &s, ptrTransaksi &transaksi) {
    if (s.Top == nullptr) {
        transaksi = nullptr;
    } else {
        transaksi = s.Top;
        s.Top = s.Top->next;
        transaksi->next = nullptr;
        saveAllTransaksiToFile(s.Top);
    }
}

void tampilkanHistori(Stack &histori) {
    loadAllTransaksiFromFile(histori.Top);
    if (histori.Top == nullptr) {
        std::cout << "Histori transaksi kosong.\n";
    } else {
        std::cout << "+--------+-----------------+----------------+-----------+----------------------+\n";
        std::cout << "| ID PS  |   Nama Pasien   | Nama Driver    |  Aksi     | Tujuan               |\n";
        std::cout << "+--------+-----------------+----------------+-----------+----------------------+\n";
    
        Stack tempStack = {nullptr};
        ptrTransaksi t;
        while (histori.Top != nullptr) {
            pop(histori, t);
            std::cout << "| " << std::setw(6) << t->pasienId << " | "
                      << std::setw(15) << std::left << t->pasienName << std::right << " | "
                      << std::setw(14) << std::left << t->ambulanceDriver << std::right << " | "
                      << std::setw(9) << std::left << t->action << std::right << " | "
                      << std::setw(20) << std::left << t->keLokasi << std::right << " |\n";
            push(tempStack, t);
        }
    
        // Kembalikan ke stack asli
        while (tempStack.Top != nullptr) {
            pop(tempStack, t);
            push(histori, t);
        }
    
        std::cout << "+--------+-----------------+----------------+-----------+----------------------+\n";
    }
}

// =========== AKHIR TRANSAKSI ============

// ============== OTHER ==============

// Fungsi menghitung jarak ambulance dengan pasien
double hitungJarak(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void prosesAmbulance(ptrAmbulance &listAmbulance, ptrRumahSakit listRS, Stack &histori, Queue &antrian) {
    // Cek apakah ada antrian pasien
    if (antrian.head == nullptr) {
        std::cout << "Tidak ada pasien dalam antrian.\n";
        return;
    }

    // Cek apakah ada ambulance yang ready
    ptrAmbulance ambulanceReady = nullptr;
    ptrAmbulance currentAmbulance = listAmbulance;
    double minDistance = -1;

    while (currentAmbulance != nullptr) {
        if (currentAmbulance->status == "Ready") {
            double distance = hitungJarak(
                currentAmbulance->locationX, currentAmbulance->locationY,
                antrian.head->locationX, antrian.head->locationY
            );
            
            if (ambulanceReady == nullptr || distance < minDistance) {
                ambulanceReady = currentAmbulance;
                minDistance = distance;
            }
        }
        currentAmbulance = currentAmbulance->next;
    }

    if (ambulanceReady == nullptr) {
        std::cout << "Tidak ada ambulance yang siap digunakan.\n";
        return;
    }

    // Proses penjemputan
    ptrPasien pasien = antrian.head;
    dequeuePasien(antrian);

    // Cari rumah sakit terdekat
    ptrRumahSakit rsTerdekat = nullptr;
    ptrRumahSakit currentRS = listRS;
    double minDistanceRS = -1;

    while (currentRS != nullptr) {
        double distance = hitungJarak(
            pasien->locationX, pasien->locationY,
            currentRS->locationX, currentRS->locationY
        );
        
        if (rsTerdekat == nullptr || distance < minDistanceRS) {
            rsTerdekat = currentRS;
            minDistanceRS = distance;
        }
        currentRS = currentRS->next;
    }

    if (rsTerdekat == nullptr) {
        std::cout << "Tidak ada rumah sakit yang tersedia.\n";
        return;
    }

    // Update status ambulance
    ambulanceReady->status = "On Duty";
    
    // Buat transaksi
    ptrTransaksi transaksi = new Transaksi{
        pasien->id,
        pasien->name,
        ambulanceReady->driverName,
        "jemput",
        rsTerdekat->name,
        nullptr
    };

    // Push ke histori
    push(histori, transaksi);

    std::cout << "\n=== PROSES PENJEMPUTAN ===\n";
    std::cout << "Ambulance: " << ambulanceReady->driverName << " (ID: " << ambulanceReady->id << ")\n";
    std::cout << "Menjemput pasien: " << pasien->name << "\n";
    std::cout << "Diantar ke: " << rsTerdekat->name << "\n";
    std::cout << "Jarak tempuh: " << std::fixed << std::setprecision(2) 
              << minDistance + minDistanceRS << " km\n";

    // Simpan perubahan status ambulance
    saveAllAmbulancesToFile(listAmbulance);
}

// ============== AKHIR OTHER =============

// ========= MAIN MENU =========
void mainMenu() {
    ptrAmbulance listAmbulance = nullptr;
    ptrPasien listPasien = nullptr;
    ptrRumahSakit listRS = nullptr;
    Queue antrianPasien = {nullptr, nullptr};
    Stack historiTransaksi = {nullptr};

    // Load data dari file saat program dimulai
    loadAllAmbulancesFromFile(listAmbulance);
    loadAllPasienFromFile(listPasien);
    loadAllRumahSakitFromFile(listRS);
    loadAllTransaksiFromFile(historiTransaksi.Top);

    int pilihan;
    do {
        std::cout << "\n=== Sistem Manajemen Ambulance ===\n";
        std::cout << "1. Tambah Ambulance\n";
        std::cout << "2. Daftar Ambulance\n";
        std::cout << "3. Tambah Pasien\n";
        std::cout << "4. Daftar Pasien\n";
        std::cout << "5. Tambah Rumah Sakit\n";
        std::cout << "6. Daftar Rumah Sakit\n";
        std::cout << "7. Masukkan Pasien ke Antrian\n";
        std::cout << "8. Tampilkan Antrian Pasien\n";
        std::cout << "9. Proses Penjemputan dan Pengantaran\n";
        std::cout << "10. Lihat Histori Transaksi\n";
        std::cout << "0. Keluar\n";
        std::cout << "Pilih: ";
        std::cin >> pilihan;
        std::cin.ignore();

        switch (pilihan) {
            case 1: {
                std::string driver, status;
                double x, y;
                
                std::cout << "Nama Driver: ";
                getline(std::cin, driver);
                std::cout << "Status (Ready/On Duty/Maintenance): ";
                getline(std::cin, status);
                std::cout << "Lokasi X: ";
                std::cin >> x;
                std::cout << "Lokasi Y: ";
                std::cin >> y;
                
                tambahAmbulance(listAmbulance, driver, status, x, y);
                break;
            }
            case 2:
                tampilkanAmbulance(listAmbulance);
                break;
            case 3: {
                std::string name, location;
                int priority;
                double x, y;
                
                std::cout << "Nama Pasien: ";
                getline(std::cin, name);
                std::cout << "Lokasi Kejadian: ";
                getline(std::cin, location);
                std::cout << "Prioritas (1-4): ";
                std::cin >> priority;
                std::cout << "Lokasi X: ";
                std::cin >> x;
                std::cout << "Lokasi Y: ";
                std::cin >> y;
                
                tambahPasien(listPasien, name, location, priority, x, y);
                break;
            }
            case 4:
                tampilkanPasien(listPasien);
                break;
            case 5: {
                std::string name;
                double x, y;
                
                std::cout << "Nama Rumah Sakit: ";
                getline(std::cin, name);
                std::cout << "Lokasi X: ";
                std::cin >> x;
                std::cout << "Lokasi Y: ";
                std::cin >> y;
                
                tambahRumahSakit(listRS, name, x, y);
                break;
            }
            case 6:
                tampilkanRumahSakit(listRS);
                break;
            case 7: {
                if (listPasien == nullptr) {
                    std::cout << "Tidak ada pasien yang bisa dimasukkan ke antrian.\n";
                    break;
                }
                
                // Tampilkan daftar pasien
                tampilkanPasien(listPasien);
                
                std::string idPasien;
                std::cout << "Masukkan ID Pasien yang akan dimasukkan ke antrian: ";
                std::cin >> idPasien;
                
                // Cari pasien
                ptrPasien p = listPasien;
                ptrPasien prev = nullptr;
                while (p != nullptr && p->id != idPasien) {
                    prev = p;
                    p = p->next;
                }
                
                if (p == nullptr) {
                    std::cout << "Pasien dengan ID " << idPasien << " tidak ditemukan.\n";
                    break;
                }
                
                // Hapus dari list pasien
                if (prev == nullptr) {
                    listPasien = p->next;
                } else {
                    prev->next = p->next;
                }
                
                // Masukkan ke antrian
                enqueuePasien(antrianPasien, p);
                std::cout << "Pasien " << p->name << " dimasukkan ke antrian.\n";
                
                // Update file
                saveAllPasienToFile(listPasien);
                break;
            }
            case 8:
                tampilkanAntrian(antrianPasien);
                break;
            case 9:
                prosesAmbulance(listAmbulance, listRS, historiTransaksi, antrianPasien);
                break;
            case 10:
                tampilkanHistori(historiTransaksi);
                break;
            case 0:
                std::cout << "Keluar dari program.\n";
                break;
            default:
                std::cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    // Bersihkan memori sebelum keluar
    while (listAmbulance != nullptr) {
        ptrAmbulance temp = listAmbulance;
        listAmbulance = listAmbulance->next;
        delete temp;
    }
    while (listPasien != nullptr) {
        ptrPasien temp = listPasien;
        listPasien = listPasien->next;
        delete temp;
    }
    while (listRS != nullptr) {
        ptrRumahSakit temp = listRS;
        listRS = listRS->next;
        delete temp;
    }
    while (historiTransaksi.Top != nullptr) {
        ptrTransaksi temp = historiTransaksi.Top;
        historiTransaksi.Top = historiTransaksi.Top->next;
        delete temp;
    }
    while (antrianPasien.head != nullptr) {
        ptrPasien temp = antrianPasien.head;
        antrianPasien.head = antrianPasien.head->next;
        delete temp;
    }
}

int main() {
    mainMenu();
    return 0;
}