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

// Struktur Rumah Sakit
struct RumahSakit
{
    std::string id;
    std::string name;
    double locationX;
    double locationY;
    RumahSakit *next;
};

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

typedef RumahSakit *ptrRumahSakit;

// Struktur Transaksi
struct Transaksi
{
    int pasienId;
    std::string pasienName;
    std::string ambulanceDriver;
    std::string action;   // "jemput" atau "antar"
    std::string keLokasi; // lokasi tujuan
    Transaksi *next;      // linked list
};

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
void traversalAmbulance(ptrAmbulance head)
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

void traversalPasien(ptrPasien head)
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

        std::string ATS[5] = {"Red", "Orange", "Green", "Blue", "White"};

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

// ============== AKHIR RUMAH SAKIT ==============

// ============== OTHER ==============

// Fungsi menghitung jarak ambulance dengan pasien
double hitungJarak(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// ============== AKHIR OTHER =============

int main()
{
    ptrAmbulance listAmbulance;
    ptrPasien listPasien;

    createListAmbulance(listAmbulance);
    createListPasien(listPasien);
    // tambahPasien(listPasien, "Ujang Bedil", "Pudal", 2, 10.1, -12.18);

    int pil;
    // while (true)
    // {
    //     std::cout << "Masukan pilihan: ";
    //     std::cin >> pil;
    //     if (pil == 1)
    //     {
    //         tambahAmbulance(listAmbulance, "budi 01", "ready", -10, 20.11);
    //         // tambahPasien(listPasien, "Ujang Bedil", "Pudal", 2, 10.1, -12.18);
    //     }
    // }
    traversalAmbulance(listAmbulance);
    // traversalPasien(listPasien);
}