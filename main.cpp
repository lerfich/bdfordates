#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <exception>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;


class Date {
public:
    Date(){}
    Date(int y, int m, int d){
        if(m < 1 || m>12) throw out_of_range("Month value is invalid: " + to_string(m));
        if(d < 1 || d>31) throw out_of_range("Day value is invalid: " + to_string(d));
        year = y; month = m; day = d;
    }
    int GetYear() const{
        return year;
    };
    int GetMonth() const{
        return month;
    };
    int GetDay() const{
        return day;
    };
private:
    int year;
    int month;
    int day;
    
};

bool operator<(const Date& lhs, const Date& rhs){
    if((lhs.GetYear() < rhs.GetYear()) || (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth()) || (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() < rhs.GetDay())) return true;
    else return false;
};

bool operator==(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay()) return true;
    else return false;
}

bool operator!=(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() != rhs.GetYear() || lhs.GetMonth() != rhs.GetMonth() || lhs.GetDay() != rhs.GetDay()) return true;
    else return false;
}

ostream& operator<<(ostream& stream, const Date& date){
    stream << setfill('0') << setw(4) << to_string(date.GetYear());
    stream << '-';
    stream << setfill('0') << setw(2) << to_string(date.GetMonth());
    stream << '-';
    stream << setfill('0') << setw(2) << to_string(date.GetDay());
    return stream;
}

Date DateReader(const string& date){
    istringstream stream(date);
    bool ItWorks = true;

    int year;
    ItWorks = ItWorks && (stream >> year);
    ItWorks = ItWorks && (stream.peek()=='-');
    stream.ignore(1);
    
    int month;
    ItWorks = ItWorks && (stream >> month);
    ItWorks = ItWorks && (stream.peek()=='-');
    stream.ignore(1);
    
    int day;
    ItWorks = ItWorks && (stream >> day);
    ItWorks = ItWorks && stream.eof();
    
    if(!ItWorks){
        throw logic_error("Wrong date format: " + date);
    }
    
    return Date(year, month, day);
}


class Database {
public:
    void AddEvent(const Date& date, const string& event){
        base[date].insert(event);
    };
    
    bool DeleteEvent(const Date& date, const string& event){
        if(base.at(date).count(event) > 0 && base.count(date) > 0){
            base[date].erase(event);
            return true;
        }
        return false;
    };

    int  DeleteDate(const Date& date){
        int to_del = 0;
        if(base.at(date).size()>0){
            to_del = base.at(date).size();
            base.erase(date);
        }
        return to_del;
    };

    set<string> Find(const Date& date) const{
        set <string> result;
        if(base.at(date).size()>0){
            result = base.at(date);
        }
        return result;
        }
    void Print() const{
        for(auto& i : base)
            for(auto& j : i.second){
                cout << i.first << ' ' << j << std::endl;
            }
    };
private:
    map<Date, set<string>> base;
};

int main() {
    try{
        Database db;
        string command;
            while (getline(cin, command)) {
                if(command == "") continue;
                
                stringstream input(command);
                
                string operation;
                input >> operation;
                
                map<string, char> Codes{{"Add", 'A'}, {"Del", 'D'}, {"Find", 'F'}, {"Print", 'P'}};
                
                char code = Codes[operation];
                switch (code){
                    case 'A':
                    {
                        string date, event;
                        input >> date >> event;
                        if(event != "" && event != " ")
                            db.AddEvent(DateReader(date),event);
                        break;
                    }
                    case 'D':
                    {
                        string date, event;
                        input >> date;
                        if(!input.eof()){
                            input >> event;
                        }
                        
                        if(event.empty()){
                            int deleted;
                            deleted = db.DeleteDate(DateReader(date));
                            cout << "Deleted " << deleted << " events" << endl;
                        }
                        else
                        {
                            if(db.DeleteEvent(DateReader(date), event)){
                                cout << "Deleted successfully" << endl;
                            }
                            else cout << "Event not found" << endl;
                        }
                        break;
                    }
                    case 'F':
                    {
                        string date;
                        input >> date;
                        set<string> result = db.Find(DateReader(date));
                        for(const auto& i : result){
                            cout << i << endl;
                        }
                        break;
                    }
                    case 'P':
                    {
                        db.Print();
                        break;
                    }
                    default:
                        cout << "Unknown command: "<< operation <<endl;
                return 0;
                }
            }
    }catch(exception& e){
        cout << e.what() << endl;
    }
    
  return 0;
}
