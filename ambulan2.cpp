#include <iostream>
#include <string>
// #include <vector>
#include <cmath>
using namespace std;

// Struktur Ambulance
struct Ambulance
{
    int id;
    string driverName;
    string status; // "Ready", "On Duty", "Maintenance"
    double locationX;
    double locationY;
    Ambulance *next; // linked list
};

// Struktur Pasien
struct Pasien
{
    int id;
    string name;
    string location;
    double locationX;
    double locationY;
    Pasien *next; // linked list
};

// Struktur Rumah Sakit
struct RumahSakit
{
    string name;
    double locationX;
    double locationY;
    RumahSakit *next;
};

// Struktur Transaksi
struct Transaksi
{
    int pasienId;
    string pasienName;
    string ambulanceDriver;
    string action;   // "jemput" atau "antar"
    string keLokasi; // lokasi tujuan
    Transaksi *next; // linked list
};

// Queue untuk antrean pasien
struct Queue
{
    Pasien *front;
    Pasien *rear;
};

// Stack untuk histori
struct Stack
{
    Transaksi *top;
};

Queue antrianPasien = {nullptr, nullptr}; // Inisialisasi queue kosong

// Fungsi menghitung jarak
double hitungJarak(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Fungsi menambahkan ambulance
void tambahAmbulance(Ambulance *&head, int id, string driver, string status, double x, double y)
{
    Ambulance *baru = new Ambulance{id, driver, status, x, y, nullptr};
    if (head == nullptr)
        head = baru;
    else
    {
        Ambulance *temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }
    cout << "Ambulance ID " << id << " ditambahkan.\n";
}

// Menampilkan ambulance
void tampilkanAmbulance(Ambulance *head)
{
    if (head == nullptr)
    {
        cout << "Daftar ambulance kosong.\n";
        return;
    }
    cout << "Daftar Ambulance:\n";
    Ambulance *temp = head;
    while (temp != nullptr)
    {
        cout << "ID: " << temp->id << " | Driver: " << temp->driverName
             << " | Status: " << temp->status
             << " | Lokasi: (" << temp->locationX << ", " << temp->locationY << ")\n";
        temp = temp->next;
    }
}

// Menambahkan pasien
void tambahPasien(Pasien *&head, int id, string name, string location, double x, double y)
{
    Pasien *baru = new Pasien{id, name, location, x, y, nullptr};
    if (head == nullptr)
        head = baru;
    else
    {
        Pasien *temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }
    cout << "Pasien " << name << " ditambahkan.\n";
}

// Menampilkan pasien
void tampilkanPasien(Pasien *head)
{
    if (head == nullptr)
    {
        cout << "Daftar pasien kosong.\n";
        return;
    }
    cout << "Daftar Pasien:\n";
    Pasien *temp = head;
    while (temp != nullptr)
    {
        cout << "ID: " << temp->id << " | Nama: " << temp->name
             << " | Lokasi: " << temp->location
             << " (" << temp->locationX << ", " << temp->locationY << ")\n";
        temp = temp->next;
    }
}

// Menambahkan rumah sakit
void tambahRumahSakit(RumahSakit *&head, string name, double x, double y)
{
    RumahSakit *baru = new RumahSakit{name, x, y, nullptr};
    if (head == nullptr)
        head = baru;
    else
    {
        RumahSakit *temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }
    cout << "Rumah Sakit " << name << " ditambahkan.\n";
}

// Menampilkan rumah sakit
void tampilkanRumahSakit(RumahSakit *head)
{
    if (head == nullptr)
    {
        cout << "Daftar Rumah Sakit kosong.\n";
        return;
    }
    cout << "Daftar Rumah Sakit:\n";
    RumahSakit *temp = head;
    while (temp != nullptr)
    {
        cout << "- " << temp->name
             << " (" << temp->locationX << ", " << temp->locationY << ")\n";
        temp = temp->next;
    }
}

// Menghitung rumah sakit terdekat
RumahSakit *cariRumahSakitTerdekat(RumahSakit *rsHead, double x, double y)
{
    RumahSakit *terdekat = nullptr;
    double jarakMin = -1;
    RumahSakit *temp = rsHead;
    while (temp != nullptr)
    {
        double jarak = hitungJarak(x, y, temp->locationX, temp->locationY);
        if (jarakMin < 0 || jarak < jarakMin)
        {
            jarakMin = jarak;
            terdekat = temp;
        }
        temp = temp->next;
    }
    return terdekat;
}

// Enqueue pasien
void enqueue(Queue &q, Pasien *p)
{
    if (q.rear == nullptr)
    {
        q.front = q.rear = p;
    }
    else
    {
        q.rear->next = p;
        q.rear = p;
    }
}

// Dequeue pasien
Pasien *dequeue(Queue &q)
{
    if (q.front == nullptr)
        return nullptr;
    Pasien *p = q.front;
    q.front = q.front->next;
    if (q.front == nullptr)
        q.rear = nullptr;
    p->next = nullptr;
    return p;
}

// Push transaksi
void push(Stack &s, Transaksi *t)
{
    t->next = s.top;
    s.top = t;
}

// Pop transaksi
Transaksi *pop(Stack &s)
{
    if (s.top == nullptr)
        return nullptr;
    Transaksi *t = s.top;
    s.top = s.top->next;
    return t;
}

// Proses penjemputan dan pengantaran
void prosesAmbulance(Ambulance *&ambulans, RumahSakit *rsList, Stack &histori, Queue &antrianPasien)
{
    if (antrianPasien.front == nullptr)
    {
        cout << "Antrean kosong.\n";
        return;
    }

    // Cari ambulance yang ready
    Ambulance *amb = ambulans;
    while (amb != nullptr && amb->status != "Ready")
        amb = amb->next;

    if (amb == nullptr)
    {
        cout << "Tidak ada ambulance siap.\n";
        return;
    }

    // Ambil pasien dari antrian (dequeue)
    Pasien *pasien = antrianPasien.front;
    antrianPasien.front = antrianPasien.front->next;

    if (antrianPasien.front == nullptr)
        antrianPasien.rear = nullptr;

    pasien->next = nullptr;

    // Tentukan rumah sakit terdekat
    RumahSakit *rsTerdekat = cariRumahSakitTerdekat(rsList, pasien->locationX, pasien->locationY);
    if (rsTerdekat == nullptr)
    {
        cout << "Tidak ada rumah sakit yang terdaftar.\n";
        // Kembalikan pasien ke antrian (enqueue)
        pasien->next = nullptr;
        if (antrianPasien.rear == nullptr)
        {
            antrianPasien.front = antrianPasien.rear = pasien;
        }
        else
        {
            antrianPasien.rear->next = pasien;
            antrianPasien.rear = pasien;
        }
        return;
    }

    // Proses penjemputan
    cout << "Ambulance ID " << amb->id << " menjemput pasien " << pasien->name
         << " di lokasi (" << pasien->locationX << ", " << pasien->locationY << ").\n";

    amb->locationX = pasien->locationX;
    amb->locationY = pasien->locationY;
    amb->status = "On Duty";

    // Catat transaksi penjemputan
    Transaksi *tJemput = new Transaksi{pasien->id, pasien->name, amb->driverName, "jemput", ""};
    push(histori, tJemput);

    // Proses pengantaran
    cout << "Mengantar pasien ke rumah sakit " << rsTerdekat->name << ".\n";
    amb->locationX = rsTerdekat->locationX;
    amb->locationY = rsTerdekat->locationY;
    amb->status = "Ready";

    // Catat transaksi pengantaran
    Transaksi *tAntar = new Transaksi{pasien->id, pasien->name, amb->driverName, "antar", rsTerdekat->name};
    push(histori, tAntar);

    delete pasien; // Hapus pasien setelah selesai diproses
}

// Main
int main()
{
    Ambulance *listAmbulance = nullptr;
    Pasien *antreanPasien = nullptr;
    RumahSakit *listRS = nullptr;
    Stack historiTransaksi = {nullptr};

    int pilihan, id;
    string driver, status, name, lokasi;
    double x, y;

    // Tambah data rumah sakit awal
    tambahRumahSakit(listRS, "RSUD Kota", 10.0, 10.0);
    tambahRumahSakit(listRS, "RS Kabupaten", 20.0, 5.0);
    tambahRumahSakit(listRS, "RS Swasta", 5.0, 15.0);

    do
    {
        cout << "\n=== Sistem Ambulance dengan Lokasi ===\n";
        cout << "1. Tambah Ambulance\n";
        cout << "2. Daftar Ambulance\n";
        cout << "3. Tambah Pasien\n";
        cout << "4. Daftar Pasien\n";
        cout << "5. Tambah Rumah Sakit\n";
        cout << "6. Daftar Rumah Sakit\n";
        cout << "7. Masukkan Pasien ke Antrean\n";
        cout << "8. Proses Penjemputan dan Pengantaran\n";
        cout << "9. Lihat Histori\n";
        cout << "10. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan)
        {
        case 1:
            cout << "ID Ambulance: ";
            cin >> id;
            cin.ignore();
            cout << "Driver Name: ";
            getline(cin, driver);
            cout << "Status (Ready/On Duty/Maintenance): ";
            getline(cin, status);
            cout << "Lokasi X: ";
            cin >> x;
            cin.ignore();
            cout << "Lokasi Y: ";
            cin >> y;
            cin.ignore();
            tambahAmbulance(listAmbulance, id, driver, status, x, y);
            break;
        case 2:
            tampilkanAmbulance(listAmbulance);
            break;
        case 3:
            cout << "ID Pasien: ";
            cin >> id;
            cin.ignore();
            cout << "Nama Pasien: ";
            getline(cin, name);
            cout << "Lokasi Pasien: ";
            getline(cin, lokasi);
            cout << "Lokasi X: ";
            cin >> x;
            cin.ignore();
            cout << "Lokasi Y: ";
            cin >> y;
            cin.ignore();
            tambahPasien(antreanPasien, id, name, lokasi, x, y);
            break;
        case 4:
            tampilkanPasien(antreanPasien);
            break;
        case 5:
            cout << "Nama Rumah Sakit: ";
            getline(cin, lokasi);
            cout << "Lokasi X Rumah Sakit: ";
            cin >> x;
            cin.ignore();
            cout << "Lokasi Y Rumah Sakit: ";
            cin >> y;
            cin.ignore();
            tambahRumahSakit(listRS, lokasi, x, y);
            break;
        case 6:
            tampilkanRumahSakit(listRS);
            break;
        case 7:
            if (antreanPasien != nullptr)
            {
                Pasien *p = antreanPasien;
                antreanPasien = antreanPasien->next;
                p->next = nullptr;

                if (antrianPasien.rear == nullptr)
                {
                    antrianPasien.front = antrianPasien.rear = p;
                }
                else
                {
                    antrianPasien.rear->next = p;
                    antrianPasien.rear = p;
                }
                cout << "Pasien masuk antrean.\n";
            }
            else
            {
                cout << "Tidak ada pasien yang bisa dimasukkan ke antrean.\n";
            }
            break;
        case 8:
            prosesAmbulance(listAmbulance, listRS, historiTransaksi, antrianPasien);
            break;
        case 9:
            // Tampilkan histori
            cout << "Histori Transaksi:\n";
            {
                Stack tempStack = {nullptr};
                while (historiTransaksi.top != nullptr)
                {
                    Transaksi *t = pop(historiTransaksi);
                    cout << t->action << ": Pasien " << t->pasienName
                         << " oleh Driver " << t->ambulanceDriver
                         << " ke " << t->keLokasi << "\n";
                    push(tempStack, t);
                }
                // Kembalikan ke stack asli
                while (tempStack.top != nullptr)
                {
                    push(historiTransaksi, pop(tempStack));
                }
            }
            break;
        case 10:
            cout << "Keluar dari program.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 10);

    return 0;
}