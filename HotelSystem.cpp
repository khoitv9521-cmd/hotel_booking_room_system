#include "HotelSystem.h"
#include <iostream>

using namespace std;

HotelSystem::HotelSystem() {
    currentUserId = -1;
    currentUserRole = "";
    currentUserName = "";
    nextCustomerId = 2;
    nextBookingId = 1;
    nextPaymentId = 1;

    admins.push_back(Admin(1, "Quan Tri Vien", "admin", "123")); 
    customers.push_back(Customer(1, "Nguyen Van A", "user", "123", "999999999", "HCM"));

    rooms.push_back(Room(101, "Phong 1", "Don", 500000, "Available"));
    rooms.push_back(Room(102, "Phong 2", "Don", 500000, "Available"));
    rooms.push_back(Room(103, "Phong 3", "Doi", 1500000, "Available"));
}

bool HotelSystem::login() {
    string email, pass;
    cout << "\n--- DANG NHAP HE THONG ---" << endl;
    cout << "Email/User: "; cin >> email;
    cout << "Mat khau: "; cin >> pass;

    for (const auto& admin : admins) {
        if (admin.email == email && admin.password == pass) {
            currentUserId = admin.adminId;
            currentUserRole = "ADMIN";
            currentUserName = admin.name;
            cout << ">> Xin chao Admin: " << admin.name << "!\n";
            return true;
        }
    }

    for (const auto& cust : customers) {
        if (cust.email == email && cust.password == pass) {
            currentUserId = cust.customerId;
            currentUserRole = "CUSTOMER";
            currentUserName = cust.name;
            cout << ">> Xin chao Khach hang: " << cust.name << "!\n";
            return true;
        }
    }

    cout << "!!! Sai email hoac mat khau.\n";
    return false;
}

void HotelSystem::registerCustomer() {
    string name, email, pass, phone, addr;
    cout << "\n--- DANG KY TAI KHOAN ---" << endl;
    cin.ignore();
    cout << "Ho ten day du: "; getline(cin, name);
    cout << "Email (Ten dang nhap): "; cin >> email;
    
    for(const auto& c : customers) {
        if(c.email == email) {
            cout << "!!! Email nay da ton tai.\n";
            return;
        }
    }

    cout << "Mat khau: "; cin >> pass;
    cout << "SDT: "; cin >> phone;
    cin.ignore();
    cout << "Dia chi: "; getline(cin, addr);

    customers.push_back(Customer(nextCustomerId++, name, email, pass, phone, addr));
    cout << ">> Dang ky thanh cong! Hay dang nhap ngay.\n";
}

void HotelSystem::logout() {
    currentUserId = -1;
    currentUserRole = "";
    currentUserName = "";
    cout << ">> Da dang xuat.\n";
}

string HotelSystem::getCustomerNameById(int id) {
    for (const auto& c : customers) {
        if (c.customerId == id) return c.name;
    }
    return "Khach Vang Lai";
}

int HotelSystem::processPayment(double amount) {
    cout << "\n--- CONG THANH TOAN ---" << endl;
    cout << "Tong tien can tra: " << (long)amount << " VND" << endl;
    cout << "Chon phuong thuc:\n1. The Tin Dung\n2. Chuyen Khoan\n3. Tien Mat\n0. Huy bo\nChon: ";
    int c; cin >> c;
    
    if (c == 0) return -1;

    string method = (c==1) ? "Credit Card" : (c==2 ? "Banking" : "Cash");
    
    cout << ">> Dang xu ly...";
    int pId = nextPaymentId++;
    payments.push_back(Payment(pId, method, amount, "Hom nay", "Completed"));
    cout << " [THANH CONG]\n";
    
    return pId;
}

void HotelSystem::adminMenu() {
    while(currentUserId != -1 && currentUserRole == "ADMIN") {
        cout << "\n=== BANG DIEU KHIEN ADMIN (" << currentUserName << ") ===" << endl;
        cout << "1. Them phong moi" << endl;
        cout << "2. Xem bao cao doanh thu" << endl;
        cout << "3. Danh sach Booking" << endl;
        cout << "0. Dang xuat" << endl;
        cout << "Chon: ";
        int choice; cin >> choice;

        switch(choice) {
            case 1: {
                int id; string name, type; double price;
                cout << "ID Phong: "; cin >> id; cin.ignore();
                cout << "Ten Phong: "; getline(cin, name);
                cout << "Loai: "; getline(cin, type);
                cout << "Gia: "; cin >> price;
                rooms.push_back(Room(id, name, type, price, "Available"));
                cout << ">> Da them phong moi.\n";
                break;
            }
            case 2: {
                double total = 0;
                cout << "\n--- BAO CAO TAI CHINH ---" << endl;
                for(auto& p : payments) {
                    if(p.status == "Completed") total += p.amount;
                }
                cout << "Tong doanh thu thuc te: " << (long)total << " VND" << endl;
                break;
            }
            case 3:
                cout << "\n--- DANH SACH DON DAT PHONG ---" << endl;
                if(bookings.empty()) cout << "Chua co don nao.\n";
                else {
                    for(auto& b : bookings) {
                        string customerName = getCustomerNameById(b.customerId);
                        cout << "-----------------------------------" << endl;
                        cout << "KHACH HANG: " << customerName << " (ID: " << b.customerId << ")" << endl;
                        b.displayBooking();
                        
                        for(auto& p : payments) {
                            if(p.paymentId == b.paymentId)
                                cout << "   -> Phuong thuc: " << p.paymentMethod << endl;
                        }
                    }
                    cout << "-----------------------------------" << endl;
                }
                break;
            case 0: logout(); return;
            default: cout << "Sai lua chon!\n";
        }
    }
}

void HotelSystem::customerMenu() {
    while(currentUserId != -1 && currentUserRole == "CUSTOMER") {
        cout << "\n=== MENU KHACH HANG (" << currentUserName << ") ===" << endl;
        cout << "1. Xem tat ca phong" << endl;
        cout << "2. Dat phong" << endl;
        cout << "3. Lich su dat phong" << endl;
        cout << "0. Dang xuat" << endl;
        cout << "Chon: ";
        int choice; cin >> choice;

        switch(choice) {
            case 1:
                cout << "\n--- DANH SACH TRANG THAI PHONG ---" << endl;
                for(auto& r : rooms) r.displayRoom();
                break;
            case 2: {
                int rId; cout << "Nhap ID phong muon dat: "; cin >> rId;
                bool found = false;
                for(auto& r : rooms) {
                    if(r.roomId == rId && r.status == "Available") {
                        string in, out;
                        cout << "Check-in: "; cin >> in;
                        cout << "Check-out: "; cin >> out;
                        
                        int pId = processPayment(r.price);
                        
                        if(pId != -1) {
                            bookings.push_back(Booking(nextBookingId++, rId, currentUserId, pId, in, out, r.price));
                            r.status = "Booked";
                            cout << ">> DAT PHONG THANH CONG!\n";
                        } else {
                            cout << ">> Huy giao dich.\n";
                        }
                        
                        found = true; break;
                    }
                }
                if(!found) cout << "!!! Phong khong ton tai hoac da co nguoi dat.\n";
                break;
            }
            case 3:
                cout << "\n--- LICH SU CUA BAN ---" << endl;
                for(auto& b : bookings) {
                    if(b.customerId == currentUserId) b.displayBooking();
                }
                break;
            case 0: logout(); return;
            default: cout << "Sai lua chon!\n";
        }
    }
}
return 0;
