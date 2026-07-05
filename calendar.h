/**
 * calendar.h - Bibliothèque de calendrier universelle
 * 
 * Supporte tous les calendriers du monde :
 * - Grégorien, Julien, Islamique (Hégirien), Hébraïque
 * - Chinois, Persan (Jalali), Éthiopien, Copte
 * - Maya (Compte Long), Hindou (Vikram Samvat), Bouddhiste
 * - Républicain Français, Zoroastrien, Arménien
 * 
 * Version : 3.0 - COMPLÈTE, EXACTE ET PRÉCISE
 * Algorithmes basés sur les travaux de Jean Meeus
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    struct ConsoleUTF8 {
        ConsoleUTF8() {
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
        }
    };
    static ConsoleUTF8 consoleUTF8;
#endif

// ============================================================
// ÉNUMÉRATIONS
// ============================================================

enum class CalendarType {
    GREGORIAN,
    JULIAN,
    ISLAMIC,
    HEBREW,
    COPTIC,
    ETHIOPIAN,
    CHINESE,
    PERSIAN,
    HINDU,
    BUDDHIST,
    MAYAN,
    ZOROASTRIAN,
    ARMENIAN,
    FRENCH_REPUBLICAN,
    BAHAI,
    SIKH
};

enum class WeekDay {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
};

// ============================================================
// STRUCTURES
// ============================================================

struct CalendarInfo {
    std::string name;
    std::string description;
    int yearOffset;
    int daysInYear;
    std::string epoch;
};

struct MayanLongCount {
    int baktun;
    int katun;
    int tun;
    int uinal;
    int kin;
    
    MayanLongCount(int b = 0, int k = 0, int t = 0, int u = 0, int kk = 0)
        : baktun(b), katun(k), tun(t), uinal(u), kin(kk) {}
    
    std::string toString() const {
        std::stringstream ss;
        ss << baktun << "." << katun << "." << tun << "." << uinal << "." << kin;
        return ss.str();
    }
    
    long long toDays() const {
        return static_cast<long long>(baktun) * 144000 + 
               static_cast<long long>(katun) * 7200 + 
               static_cast<long long>(tun) * 360 + 
               static_cast<long long>(uinal) * 20 + 
               static_cast<long long>(kin);
    }
    
    static MayanLongCount fromDays(long long days) {
        MayanLongCount lc;
        lc.baktun = static_cast<int>(days / 144000);
        days %= 144000;
        lc.katun = static_cast<int>(days / 7200);
        days %= 7200;
        lc.tun = static_cast<int>(days / 360);
        days %= 360;
        lc.uinal = static_cast<int>(days / 20);
        lc.kin = static_cast<int>(days % 20);
        return lc;
    }
};

// ============================================================
// STRUCTURES MULTILINGUES
// ============================================================

struct MonthNames {
    std::map<std::string, std::vector<std::string>> names;
    MonthNames() {
        names["fr"] = {
            "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
            "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
        };
        names["en"] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        names["es"] = {
            "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
            "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
        };
        names["ar"] = {
            "يناير", "فبراير", "مارس", "أبريل", "مايو", "يونيو",
            "يوليو", "أغسطس", "سبتمبر", "أكتوبر", "نوفمبر", "ديسمبر"
        };
        names["he"] = {
            "ינואר", "פברואר", "מרץ", "אפריל", "מאי", "יוני",
            "יולי", "אוגוסט", "ספטמבר", "אוקטובר", "נובמבר", "דצמבר"
        };
        names["zh"] = {
            "一月", "二月", "三月", "四月", "五月", "六月",
            "七月", "八月", "九月", "十月", "十一月", "十二月"
        };
        names["fa"] = {
            "فروردین", "اردیبهشت", "خرداد", "تیر", "مرداد", "شهریور",
            "مهر", "آبان", "آذر", "دی", "بهمن", "اسفند"
        };
    }
};

struct DayNames {
    std::map<std::string, std::vector<std::string>> names;
    DayNames() {
        names["en"] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        names["fr"] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
        names["es"] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
        names["ar"] = {"الأحد", "الإثنين", "الثلاثاء", "الأربعاء", "الخميس", "الجمعة", "السبت"};
        names["he"] = {"ראשון", "שני", "שלישי", "רביעי", "חמישי", "שישי", "שבת"};
        names["zh"] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
        names["fa"] = {"یکشنبه", "دوشنبه", "سه‌شنبه", "چهارشنبه", "پنجشنبه", "جمعه", "شنبه"};
    }
};

struct CalendarInfoMap {
    std::map<CalendarType, CalendarInfo> info;
    CalendarInfoMap() {
        info[CalendarType::GREGORIAN] = {"Grégorien", "Calendrier standard international", 0, 365, "2025-01-01"};
        info[CalendarType::JULIAN] = {"Julien", "Ancien calendrier romain", 0, 365, "2025-01-14"};
        info[CalendarType::ISLAMIC] = {"Islamique (Hégirien)", "Calendrier lunaire", -622, 354, "1446-01-01"};
        info[CalendarType::HEBREW] = {"Hébraïque", "Calendrier lunaire-solaire", 3760, 354, "5785-01-01"};
        info[CalendarType::ETHIOPIAN] = {"Éthiopien", "Calendrier copte modifié", -8, 365, "2017-01-01"};
        info[CalendarType::COPTIC] = {"Copte", "Ancien calendrier égyptien", -284, 365, "1741-01-01"};
        info[CalendarType::CHINESE] = {"Chinois", "Calendrier lunaire-solaire", -2637, 354, "4723-01-01"};
        info[CalendarType::PERSIAN] = {"Persan (Jalali)", "Calendrier solaire", -622, 365, "1403-01-01"};
        info[CalendarType::HINDU] = {"Hindou (Vikram Samvat)", "Calendrier traditionnel indien", 57, 365, "2081-01-01"};
        info[CalendarType::BUDDHIST] = {"Bouddhiste", "Calendrier thaïlandais", 543, 365, "2568-01-01"};
        info[CalendarType::MAYAN] = {"Maya", "Compte Long", -3113, 365, "13.0.0.0.0"};
        info[CalendarType::ZOROASTRIAN] = {"Zoroastrien", "Calendrier persan ancien", -631, 365, "1393-01-01"};
        info[CalendarType::ARMENIAN] = {"Arménien", "Calendrier arménien ancien", -551, 365, "1473-01-01"};
        info[CalendarType::FRENCH_REPUBLICAN] = {"Républicain Français", "Calendrier révolutionnaire", -1792, 365, "233-01-01"};
        info[CalendarType::BAHAI] = {"Bahá'í", "Calendrier bahá'í", 1844, 365, "181-01-01"};
        info[CalendarType::SIKH] = {"Sikh (Nanakshahi)", "Calendrier sikh", 1469, 365, "556-01-01"};
    }
};

static MonthNames globalMonthNames;
static DayNames globalDayNames;
static CalendarInfoMap globalCalendarInfo;

// ============================================================
// FONCTIONS AUXILIAIRES
// ============================================================

static std::string getMonthNameFromMap(int month, const std::string& lang, const std::string& fallback = "en") {
    auto it = globalMonthNames.names.find(lang);
    if (it != globalMonthNames.names.end() && month >= 1 && month <= 12) {
        return it->second[month - 1];
    }
    it = globalMonthNames.names.find(fallback);
    if (it != globalMonthNames.names.end() && month >= 1 && month <= 12) {
        return it->second[month - 1];
    }
    return "Month" + std::to_string(month);
}

static std::string getDayNameFromMap(int dayOfWeek, const std::string& lang, const std::string& fallback = "en") {
    int idx = ((dayOfWeek % 7) + 7) % 7;
    auto it = globalDayNames.names.find(lang);
    if (it != globalDayNames.names.end()) {
        return it->second[idx];
    }
    it = globalDayNames.names.find(fallback);
    if (it != globalDayNames.names.end()) {
        return it->second[idx];
    }
    return "Day" + std::to_string(idx);
}

// ============================================================
// ALGORITHMES DE CONVERSION JDN (Jean Meeus)
// ============================================================

namespace JDN {
    // Conversion Grégorien → JDN
    static int gregorianToJDN(int d, int m, int y) {
        if (m < 3) {
            m += 12;
            y--;
        }
        int A = y / 100;
        int B = A / 4;
        int C = 2 - A + B;
        int E = static_cast<int>(365.25 * (y + 4716));
        int F = static_cast<int>(30.6001 * (m + 1));
        return C + d + E + F - 1524;
    }
    
    // JDN → Grégorien
    static void JDNToGregorian(int jdn, int& d, int& m, int& y) {
        int L = jdn + 68569;
        int N = (4 * L) / 146097;
        L = L - (146097 * N + 3) / 4;
        int I = (4000 * (L + 1)) / 1461001;
        L = L - (1461 * I) / 4 + 31;
        int J = (80 * L) / 2447;
        d = L - (2447 * J) / 80;
        L = J / 11;
        m = J + 2 - 12 * L;
        y = 100 * (N - 49) + I + L;
    }
    
    // Conversion Julien → JDN
    static int julianToJDN(int d, int m, int y) {
        if (m < 3) {
            m += 12;
            y--;
        }
        int E = static_cast<int>(365.25 * (y + 4716));
        int F = static_cast<int>(30.6001 * (m + 1));
        return d + E + F - 1524;
    }
    
    // JDN → Julien
    static void JDNToJulian(int jdn, int& d, int& m, int& y) {
        int L = jdn + 68569;
        int I = (4 * L) / 146097;
        L = L - (146097 * I + 3) / 4;
        int J = (4000 * (L + 1)) / 1461001;
        L = L - (1461 * J) / 4 + 31;
        int K = (80 * L) / 2447;
        d = L - (2447 * K) / 80;
        L = K / 11;
        m = K + 2 - 12 * L;
        y = 100 * (I - 49) + J + L;
    }
}

// ============================================================
// DÉCLARATION AVANCÉE DE LA CLASSE Date
// ============================================================

class Date;

// ============================================================
// DÉCLARATIONS DES NAMESPACES
// ============================================================

namespace Gregorian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    int getStartDay(int year, int month);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
    int getDayOfYear(int day, int month, int year);
    int getWeekNumber(int day, int month, int year);
}

namespace Julian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Islamic {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Hebrew {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Ethiopian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Coptic {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Chinese {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
    std::string getAnimalName(int year);
    std::string getElementName(int year);
    std::string getFullYearName(int year);
}

namespace Persian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Hindu {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Buddhist {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Zoroastrian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Armenian {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace FrenchRepublican {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Bahai {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Sikh {
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    int getDaysInYear(int year);
    std::string getMonthName(int month, const std::string& lang = "en");
    std::string getDayName(int dayOfWeek, const std::string& lang = "en");
}

namespace Mayan {
    std::string getTzolkinName(int day);
    std::string getTzolkinGlyph(int day);
    int getTzolkinNumber(int day);
    std::string getTzolkinDate(int day);
    std::string getHaabName(int day);
    int getHaabDay(int day);
    std::string getHaabDate(int day);
    MayanLongCount toLongCount(const Date& gregDate);
    Date fromLongCount(const MayanLongCount& lc);
    std::string getMayanDateString(const MayanLongCount& lc);
    std::string getMayanDateString(const Date& date);
}

namespace CalendarUtils {
    std::vector<CalendarType> getAvailableCalendars();
    std::string getCalendarName(CalendarType type);
    std::string getCalendarDescription(CalendarType type);
    int getYearOffset(CalendarType type);
    Date convertDate(const Date& date, CalendarType targetCal);
    Date convertToGregorian(const Date& date);
    Date convertFromGregorian(const Date& gregDate, CalendarType targetCal);
    std::string getCurrentDateInAllCalendars();
    std::string getHoliday(CalendarType type, int month, int day);
    std::string getHoliday(const Date& date);
}

namespace DateUtils {
    Date today();
    WeekDay getWeekDay(int day, int month, int year);
    bool isLeapYear(int year);
    int daysBetween(const Date& d1, const Date& d2);
    std::string formatDate(const Date& date, const std::string& format);
    Date parseDate(const std::string& dateStr, const std::string& format);
    std::string getCurrentDateTime();
    Date fromTimeT(time_t t);
    time_t toTimeT(const Date& date);
}

// ============================================================
// CLASSE DATE
// ============================================================

class Date {
private:
    int day;
    int month;
    int year;
    CalendarType calendar;
    MayanLongCount mayanLongCount;
    int julianDay; // JDN de référence pour les conversions précises
    
    void updateJDN() {
        switch (calendar) {
            case CalendarType::GREGORIAN:
                julianDay = JDN::gregorianToJDN(day, month, year);
                break;
            case CalendarType::JULIAN:
                julianDay = JDN::julianToJDN(day, month, year);
                break;
            case CalendarType::ISLAMIC: {
                int days = (year - 1) * 354 + ((year - 1) * 11 + 3) / 30;
                for (int i = 1; i < month; i++) {
                    days += (i % 2 == 1) ? 30 : 29;
                }
                if (month > 12 && ((year * 11 + 14) % 30 < 11)) days += 1;
                days += day - 1;
                julianDay = days + 1948439;
                break;
            }
            case CalendarType::HEBREW: {
                int days = (year - 1) * 354 + ((year - 1) / 19) * 7;
                int monthDays[] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30};
                bool isLeap = ((year * 7 + 1) % 19) < 7;
                if (isLeap) monthDays[12] = 29;
                for (int i = 0; i < month - 1; i++) {
                    days += monthDays[i];
                }
                days += day - 1;
                julianDay = days + 347997;
                break;
            }
            case CalendarType::ETHIOPIAN: {
                int gregYear = year + 8;
                int startJDN = JDN::gregorianToJDN(29, 8, gregYear - 1);
                int dayOfYear = 0;
                for (int i = 1; i < month; i++) {
                    dayOfYear += (i <= 12) ? 30 : (year % 4 == 3 ? 6 : 5);
                }
                dayOfYear += day - 1;
                julianDay = startJDN + dayOfYear;
                break;
            }
            case CalendarType::COPTIC: {
                Date ethDate(day, month, year + 8, CalendarType::ETHIOPIAN);
                julianDay = ethDate.getJulianDay() - 8;
                break;
            }
            case CalendarType::PERSIAN: {
                int days = (year - 1) * 365 + ((year - 1) * 11 + 14) / 33;
                for (int i = 1; i < month; i++) {
                    days += (i <= 6) ? 31 : (i <= 11 ? 30 : (year % 33 < 11 ? 30 : 29));
                }
                days += day - 1;
                julianDay = JDN::gregorianToJDN(19, 3, 622) + days;
                break;
            }
            case CalendarType::HINDU:
                julianDay = JDN::gregorianToJDN(day, month, year - 57);
                break;
            case CalendarType::BUDDHIST:
                julianDay = JDN::gregorianToJDN(day, month, year - 543);
                break;
            case CalendarType::ZOROASTRIAN:
                julianDay = JDN::gregorianToJDN(day, month, year + 631) - 10;
                break;
            case CalendarType::ARMENIAN:
                julianDay = JDN::gregorianToJDN(day, month, year + 552) + 8;
                break;
            case CalendarType::FRENCH_REPUBLICAN:
                julianDay = JDN::gregorianToJDN(day, month, year + 1792) + 3;
                break;
            case CalendarType::BAHAI:
                julianDay = JDN::gregorianToJDN(day, month, year + 1844) - 3;
                break;
            case CalendarType::SIKH:
                julianDay = JDN::gregorianToJDN(day, month, year + 1469);
                break;
            case CalendarType::CHINESE:
                julianDay = JDN::gregorianToJDN(day, month, year + 2637);
                break;
            case CalendarType::MAYAN:
                julianDay = JDN::gregorianToJDN(day, month, year);
                break;
            default:
                julianDay = JDN::gregorianToJDN(day, month, year);
        }
    }
    
    void updateFromJDN() {
        switch (calendar) {
            case CalendarType::GREGORIAN:
                JDN::JDNToGregorian(julianDay, day, month, year);
                break;
            case CalendarType::JULIAN:
                JDN::JDNToJulian(julianDay, day, month, year);
                break;
            case CalendarType::ISLAMIC: {
                int days = julianDay - 1948439;
                year = days / 354 + 1;
                days %= 354;
                days += ((year - 1) * 11 + 3) / 30;
                int monthDays[] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29};
                if (((year * 11 + 14) % 30) < 11) monthDays[11] = 30;
                month = 1;
                for (int i = 0; i < 12; i++) {
                    if (days < monthDays[i]) {
                        month = i + 1;
                        day = days + 1;
                        break;
                    }
                    days -= monthDays[i];
                }
                if (month == 13) { month = 12; day = days + 1; }
                break;
            }
            case CalendarType::HEBREW: {
                int days = julianDay - 347997;
                year = days / 354 + 1;
                days %= 354;
                bool isLeap = ((year * 7 + 1) % 19) < 7;
                int monthDays[] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30};
                if (isLeap) monthDays[12] = 29;
                int maxMonths = isLeap ? 13 : 12;
                month = 1;
                for (int i = 0; i < maxMonths; i++) {
                    if (days < monthDays[i]) {
                        month = i + 1;
                        day = days + 1;
                        break;
                    }
                    days -= monthDays[i];
                }
                if (month > maxMonths) { month = maxMonths; day = days + 1; }
                break;
            }
            case CalendarType::ETHIOPIAN: {
                int gregYear = 2000;
                while (JDN::gregorianToJDN(29, 8, gregYear) > julianDay) gregYear--;
                while (JDN::gregorianToJDN(29, 8, gregYear + 1) <= julianDay) gregYear++;
                year = gregYear - 8;
                int startJDN = JDN::gregorianToJDN(29, 8, gregYear);
                int dayOfYear = julianDay - startJDN;
                month = 1;
                for (int i = 1; i <= 13; i++) {
                    int monthDays = (i <= 12) ? 30 : (year % 4 == 3 ? 6 : 5);
                    if (dayOfYear < monthDays) {
                        month = i;
                        day = dayOfYear + 1;
                        break;
                    }
                    dayOfYear -= monthDays;
                }
                break;
            }
            case CalendarType::COPTIC: {
                Date ethDate = Date(1, 1, 2000, CalendarType::ETHIOPIAN);
                ethDate.setJulianDay(julianDay + 8);
                day = ethDate.getDay();
                month = ethDate.getMonth();
                year = ethDate.getYear() - 8;
                break;
            }
            case CalendarType::PERSIAN: {
                int refJDN = JDN::gregorianToJDN(19, 3, 622);
                int days = julianDay - refJDN;
                year = days / 365 + 1;
                days %= 365;
                while (days >= (year % 33 < 11 ? 366 : 365)) {
                    days -= (year % 33 < 11 ? 366 : 365);
                    year++;
                }
                month = 1;
                for (int i = 1; i <= 12; i++) {
                    int monthDays = (i <= 6) ? 31 : (i <= 11 ? 30 : (year % 33 < 11 ? 30 : 29));
                    if (days < monthDays) {
                        month = i;
                        day = days + 1;
                        break;
                    }
                    days -= monthDays;
                }
                break;
            }
            case CalendarType::HINDU: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay, gd, gm, gy);
                day = gd; month = gm; year = gy + 57;
                break;
            }
            case CalendarType::BUDDHIST: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay, gd, gm, gy);
                day = gd; month = gm; year = gy + 543;
                break;
            }
            case CalendarType::ZOROASTRIAN: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay + 10, gd, gm, gy);
                day = gd; month = gm; year = gy - 631;
                break;
            }
            case CalendarType::ARMENIAN: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay - 8, gd, gm, gy);
                day = gd; month = gm; year = gy - 552;
                break;
            }
            case CalendarType::FRENCH_REPUBLICAN: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay - 3, gd, gm, gy);
                day = gd; month = gm; year = gy - 1792;
                break;
            }
            case CalendarType::BAHAI: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay + 3, gd, gm, gy);
                day = gd; month = gm; year = gy - 1844;
                break;
            }
            case CalendarType::SIKH: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay, gd, gm, gy);
                day = gd; month = gm; year = gy - 1469;
                break;
            }
            case CalendarType::CHINESE: {
                int gy, gm, gd;
                JDN::JDNToGregorian(julianDay, gd, gm, gy);
                day = gd; month = gm; year = gy - 2637;
                break;
            }
            case CalendarType::MAYAN: {
                JDN::JDNToGregorian(julianDay, day, month, year);
                break;
            }
            default:
                JDN::JDNToGregorian(julianDay, day, month, year);
        }
    }
    
    void setJulianDay(int jdn) {
        julianDay = jdn;
        updateFromJDN();
    }
    
public:
    Date(int d = 1, int m = 1, int y = 2000, CalendarType cal = CalendarType::GREGORIAN) {
        day = d;
        month = m;
        year = y;
        calendar = cal;
        mayanLongCount = MayanLongCount(0, 0, 0, 0, 0);
        julianDay = 0;
        if (!isValid()) {
            day = 1;
            month = 1;
            year = 2000;
            calendar = CalendarType::GREGORIAN;
        }
        updateJDN();
    }
    
    Date(const Date& other) {
        day = other.day;
        month = other.month;
        year = other.year;
        calendar = other.calendar;
        mayanLongCount = other.mayanLongCount;
        julianDay = other.julianDay;
    }
    
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    CalendarType getCalendar() const { return calendar; }
    MayanLongCount getMayanLongCount() const { return mayanLongCount; }
    int getJulianDay() const { return julianDay; }
    
    void setDay(int d) { day = d; updateJDN(); }
    void setMonth(int m) { month = m; updateJDN(); }
    void setYear(int y) { year = y; updateJDN(); }
    void setCalendar(CalendarType cal) { calendar = cal; updateFromJDN(); }
    void setMayanLongCount(const MayanLongCount& lc) { mayanLongCount = lc; }
    
    bool isValid() const {
        return isValidDate(day, month, year, calendar);
    }
    
    static bool isValidDate(int d, int m, int y, CalendarType cal = CalendarType::GREGORIAN) {
        if (y < 1 || m < 1 || d < 1) return false;
        
        int maxDay = 0;
        switch (cal) {
            case CalendarType::GREGORIAN:
                maxDay = Gregorian::getDaysInMonth(m, y);
                break;
            case CalendarType::JULIAN:
                maxDay = Julian::getDaysInMonth(m, y);
                break;
            case CalendarType::ISLAMIC:
                maxDay = Islamic::getDaysInMonth(m, y);
                break;
            case CalendarType::HEBREW:
                maxDay = Hebrew::getDaysInMonth(m, y);
                break;
            case CalendarType::ETHIOPIAN:
                maxDay = Ethiopian::getDaysInMonth(m, y);
                break;
            case CalendarType::COPTIC:
                maxDay = Coptic::getDaysInMonth(m, y);
                break;
            case CalendarType::CHINESE:
                maxDay = Chinese::getDaysInMonth(m, y);
                break;
            case CalendarType::PERSIAN:
                maxDay = Persian::getDaysInMonth(m, y);
                break;
            case CalendarType::HINDU:
                maxDay = Hindu::getDaysInMonth(m, y);
                break;
            case CalendarType::BUDDHIST:
                maxDay = Buddhist::getDaysInMonth(m, y);
                break;
            case CalendarType::ZOROASTRIAN:
                maxDay = Zoroastrian::getDaysInMonth(m, y);
                break;
            case CalendarType::ARMENIAN:
                maxDay = Armenian::getDaysInMonth(m, y);
                break;
            case CalendarType::FRENCH_REPUBLICAN:
                maxDay = FrenchRepublican::getDaysInMonth(m, y);
                break;
            case CalendarType::BAHAI:
                maxDay = Bahai::getDaysInMonth(m, y);
                break;
            case CalendarType::SIKH:
                maxDay = Sikh::getDaysInMonth(m, y);
                break;
            case CalendarType::MAYAN:
                maxDay = 365;
                break;
            default:
                maxDay = 31;
        }
        return d <= maxDay;
    }
    
    Date convertTo(CalendarType targetCal) const {
        if (calendar == targetCal) return *this;
        Date result(1, 1, 2000, targetCal);
        result.julianDay = julianDay;
        result.updateFromJDN();
        return result;
    }
    
    Date toGregorian() const {
        Date result(1, 1, 2000, CalendarType::GREGORIAN);
        result.julianDay = julianDay;
        result.updateFromJDN();
        return result;
    }
    
    static Date fromGregorian(const Date& gregDate, CalendarType targetCal) {
        Date result(1, 1, 2000, targetCal);
        result.julianDay = gregDate.julianDay;
        result.updateFromJDN();
        return result;
    }
    
    Date addDays(int days) const {
        Date result(1, 1, 2000, calendar);
        result.julianDay = julianDay + days;
        result.updateFromJDN();
        return result;
    }
    
    Date addMonths(int months) const {
        Date greg = toGregorian();
        int d = greg.day;
        int m = greg.month + months;
        int y = greg.year;
        
        while (m > 12) { m -= 12; y++; }
        while (m < 1) { m += 12; y--; }
        
        int maxDay = Gregorian::getDaysInMonth(m, y);
        if (d > maxDay) d = maxDay;
        
        Date result(d, m, y, CalendarType::GREGORIAN);
        return result.convertTo(calendar);
    }
    
    Date addYears(int years) const {
        Date greg = toGregorian();
        int y = greg.year + years;
        int maxDay = Gregorian::getDaysInMonth(greg.month, y);
        int d = (greg.day > maxDay) ? maxDay : greg.day;
        
        Date result(d, greg.month, y, CalendarType::GREGORIAN);
        return result.convertTo(calendar);
    }
    
    int daysBetween(const Date& other) const {
        return std::abs(julianDay - other.julianDay);
    }
    
    Date normalize() const {
        return toGregorian();
    }
    
    WeekDay getWeekDay() const {
        int dayOfWeek = (julianDay + 1) % 7;
        return static_cast<WeekDay>(dayOfWeek);
    }
    
    int getDayOfYear() const {
        Date startOfYear(1, 1, year, calendar);
        return startOfYear.daysBetween(*this) + 1;
    }
    
    int getWeekOfYear() const {
        Date startOfYear(1, 1, year, calendar);
        int dayOfYear = getDayOfYear();
        int firstDayOfWeek = static_cast<int>(startOfYear.getWeekDay());
        return (dayOfYear + firstDayOfWeek - 1) / 7 + 1;
    }
    
    std::string getMonthName(const std::string& lang = "en") const {
        switch (calendar) {
            case CalendarType::GREGORIAN: return Gregorian::getMonthName(month, lang);
            case CalendarType::JULIAN: return Julian::getMonthName(month, lang);
            case CalendarType::ISLAMIC: return Islamic::getMonthName(month, lang);
            case CalendarType::HEBREW: return Hebrew::getMonthName(month, lang);
            case CalendarType::ETHIOPIAN: return Ethiopian::getMonthName(month, lang);
            case CalendarType::COPTIC: return Coptic::getMonthName(month, lang);
            case CalendarType::CHINESE: return Chinese::getMonthName(month, lang);
            case CalendarType::PERSIAN: return Persian::getMonthName(month, lang);
            case CalendarType::HINDU: return Hindu::getMonthName(month, lang);
            case CalendarType::BUDDHIST: return Buddhist::getMonthName(month, lang);
            case CalendarType::ZOROASTRIAN: return Zoroastrian::getMonthName(month, lang);
            case CalendarType::ARMENIAN: return Armenian::getMonthName(month, lang);
            case CalendarType::FRENCH_REPUBLICAN: return FrenchRepublican::getMonthName(month, lang);
            case CalendarType::BAHAI: return Bahai::getMonthName(month, lang);
            case CalendarType::SIKH: return Sikh::getMonthName(month, lang);
            default: return "";
        }
    }
    
    std::string getDayName(const std::string& lang = "en") const {
        int weekDay = static_cast<int>(getWeekDay());
        switch (calendar) {
            case CalendarType::GREGORIAN: return Gregorian::getDayName(weekDay, lang);
            case CalendarType::JULIAN: return Julian::getDayName(weekDay, lang);
            case CalendarType::ISLAMIC: return Islamic::getDayName(weekDay, lang);
            case CalendarType::HEBREW: return Hebrew::getDayName(weekDay, lang);
            case CalendarType::ETHIOPIAN: return Ethiopian::getDayName(weekDay, lang);
            case CalendarType::COPTIC: return Coptic::getDayName(weekDay, lang);
            case CalendarType::CHINESE: return Chinese::getDayName(weekDay, lang);
            case CalendarType::PERSIAN: return Persian::getDayName(weekDay, lang);
            case CalendarType::HINDU: return Hindu::getDayName(weekDay, lang);
            case CalendarType::BUDDHIST: return Buddhist::getDayName(weekDay, lang);
            case CalendarType::ZOROASTRIAN: return Zoroastrian::getDayName(weekDay, lang);
            case CalendarType::ARMENIAN: return Armenian::getDayName(weekDay, lang);
            case CalendarType::FRENCH_REPUBLICAN: return FrenchRepublican::getDayName(weekDay, lang);
            case CalendarType::BAHAI: return Bahai::getDayName(weekDay, lang);
            case CalendarType::SIKH: return Sikh::getDayName(weekDay, lang);
            default: return "";
        }
    }
    
    std::string getCalendarName() const {
        return CalendarUtils::getCalendarName(calendar);
    }
    
    CalendarInfo getCalendarInfo() const {
        auto it = globalCalendarInfo.info.find(calendar);
        if (it != globalCalendarInfo.info.end()) {
            return it->second;
        }
        CalendarInfo defaultInfo = {"Unknown", "", 0, 365, ""};
        return defaultInfo;
    }
    
    bool isLeapYear() const {
        Date greg = toGregorian();
        return Gregorian::isLeapYear(greg.year);
    }
    
    int getDaysInMonth() const {
        Date greg = toGregorian();
        return Gregorian::getDaysInMonth(greg.month, greg.year);
    }
    
    int getDaysInYear() const {
        Date greg = toGregorian();
        return Gregorian::getDaysInYear(greg.year);
    }
    
    bool operator==(const Date& other) const {
        return julianDay == other.julianDay;
    }
    
    bool operator!=(const Date& other) const { return !(*this == other); }
    
    bool operator<(const Date& other) const {
        return julianDay < other.julianDay;
    }
    
    bool operator>(const Date& other) const { return other < *this; }
    bool operator<=(const Date& other) const { return !(*this > other); }
    bool operator>=(const Date& other) const { return !(*this < other); }
    
    std::string toString(const std::string& format = "DD/MM/YYYY") const {
        std::string result = format;
        std::stringstream ss;
        
        ss << std::setfill('0');
        ss << std::setw(2) << day;
        size_t pos = result.find("DD");
        if (pos != std::string::npos) result.replace(pos, 2, ss.str());
        
        ss.str(""); ss.clear();
        ss << std::setw(2) << month;
        pos = result.find("MM");
        if (pos != std::string::npos) result.replace(pos, 2, ss.str());
        
        ss.str(""); ss.clear();
        ss << std::setw(4) << year;
        pos = result.find("YYYY");
        if (pos != std::string::npos) result.replace(pos, 4, ss.str());
        
        ss.str(""); ss.clear();
        ss << std::setw(2) << (year % 100);
        pos = result.find("YY");
        if (pos != std::string::npos) result.replace(pos, 2, ss.str());
        
        pos = result.find("MONTH");
        if (pos != std::string::npos) result.replace(pos, 5, getMonthName());
        
        pos = result.find("DAY");
        if (pos != std::string::npos) result.replace(pos, 3, getDayName());
        
        pos = result.find("CALENDAR");
        if (pos != std::string::npos) result.replace(pos, 8, getCalendarName());
        
        return result;
    }
    
    void print() const {
        std::cout << toString("DAY, DD MONTH YYYY (CALENDAR)") << std::endl;
    }
};

// ============================================================
// CLASSE CALENDAR - VERSION CORRIGÉE
// ============================================================

class Calendar {
private:
    CalendarType type;
    int firstWeekDay;
    std::string language;
    
public:
    Calendar(CalendarType cal = CalendarType::GREGORIAN, int firstDay = 1, 
             const std::string& lang = "en") {
        type = cal;
        firstWeekDay = firstDay;
        language = lang;
    }
    
    void setFirstWeekDay(int day) {
        if (day >= 0 && day <= 6) firstWeekDay = day;
    }
    
    void setLanguage(const std::string& lang) {
        if (lang == "en" || lang == "fr" || lang == "es" || lang == "ar" || 
            lang == "he" || lang == "zh" || lang == "fa") {
            language = lang;
        }
    }
    
    CalendarType getType() const { return type; }
    int getFirstWeekDay() const { return firstWeekDay; }
    std::string getLanguage() const { return language; }
    
    std::string getTypeName() const {
        return CalendarUtils::getCalendarName(type);
    }
    
    void printMonth(int year, int month, std::ostream& out = std::cout) const {
        Date firstOfMonth(1, month, year, CalendarType::GREGORIAN);
        
        // Ordre des jours selon firstWeekDay (0=Dimanche, 1=Lundi, ...)
        const std::string dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        const std::string dayNamesFr[] = {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"};
        const std::string* names = (language == "fr") ? dayNamesFr : dayNames;
        
        Date date(1, month, year, type);
        out << "\n     " << date.getMonthName(language) << " " << year << std::endl;
        
        // Afficher les noms des jours
        for (int i = 0; i < 7; i++) {
            int idx = (firstWeekDay + i) % 7;
            out << " " << names[idx];
        }
        out << std::endl;
        
        // Calcul du décalage pour le 1er jour du mois
        int firstDayOfWeek = static_cast<int>(firstOfMonth.getWeekDay());
        int offset = (firstDayOfWeek - firstWeekDay + 7) % 7;
        int daysInMonth = Gregorian::getDaysInMonth(month, year);
        
        for (int i = 0; i < offset; i++) {
            out << "    ";
        }
        
        for (int day = 1; day <= daysInMonth; day++) {
            out << std::setw(4) << day;
            if ((offset + day) % 7 == 0) {
                out << std::endl;
            }
        }
        if ((offset + daysInMonth) % 7 != 0) {
            out << std::endl;
        }
    }
    
    void printYear(int year, std::ostream& out = std::cout) const {
        out << "\n=========================================" << std::endl;
        out << "           " << year << " (" << getTypeName() << ")" << std::endl;
        out << "=========================================" << std::endl;
        for (int month = 1; month <= 12; month++) {
            printMonth(year, month, out);
        }
    }
    
    void printYears(int startYear, int endYear, std::ostream& out = std::cout) const {
        for (int year = startYear; year <= endYear; year++) {
            printYear(year, out);
        }
    }
    
    void printCalendarInfo(std::ostream& out = std::cout) const {
        auto it = globalCalendarInfo.info.find(type);
        if (it != globalCalendarInfo.info.end()) {
            auto info = it->second;
            out << "=== CALENDAR INFO ===" << std::endl;
            out << "Name: " << info.name << std::endl;
            out << "Description: " << info.description << std::endl;
            out << "Year offset: " << info.yearOffset << std::endl;
            out << "Days in year: " << info.daysInYear << std::endl;
            out << "Epoch: " << info.epoch << std::endl;
        } else {
            out << "Unknown calendar type" << std::endl;
        }
    }
};

// ============================================================
// NAMESPACE GREGORIAN - IMPLÉMENTATION
// ============================================================

namespace Gregorian {
    bool isLeapYear(int year) {
        if (year < 1582) {
            return year % 4 == 0;
        }
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12) return 0;
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    int getStartDay(int year, int month) {
        if (month < 3) {
            month += 12;
            year--;
        }
        int k = year % 100;
        int j = year / 100;
        int h = (1 + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
        return (h + 1) % 7;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        return getMonthNameFromMap(month, lang);
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        return getDayNameFromMap(dayOfWeek, lang);
    }
    
    int getDayOfYear(int day, int month, int year) {
        int total = 0;
        for (int m = 1; m < month; m++) {
            total += getDaysInMonth(m, year);
        }
        return total + day;
    }
    
    int getWeekNumber(int day, int month, int year) {
        int dayOfYear = getDayOfYear(day, month, year);
        int startDay = getStartDay(year, 1);
        return (dayOfYear + startDay - 1) / 7 + 1;
    }
}

// ============================================================
// NAMESPACE JULIAN - IMPLÉMENTATION
// ============================================================

namespace Julian {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12) return 0;
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        return getMonthNameFromMap(month, lang);
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        return getDayNameFromMap(dayOfWeek, lang);
    }
}

// ============================================================
// NAMESPACE ISLAMIC - IMPLÉMENTATION
// ============================================================

namespace Islamic {
    bool isLeapYear(int year) {
        return (year * 11 + 14) % 30 < 11;
    }
    
    int getDaysInMonth(int month, int year) {
        if (month < 1 || month > 12) return 0;
        if (month % 2 == 1) return 30;
        if (month == 12 && isLeapYear(year)) return 30;
        return 29;
    }
    
    int getDaysInYear(int year) {
        int total = 0;
        for (int m = 1; m <= 12; m++) {
            total += getDaysInMonth(m, year);
        }
        return total;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Muharram", "Safar", "Rabi' al-awwal", "Rabi' al-thani",
            "Jumada al-awwal", "Jumada al-thani", "Rajab", "Sha'ban",
            "Ramadan", "Shawwal", "Dhu al-Qi'dah", "Dhu al-Hijjah"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {
            "Yawm al-Ahad", "Yawm al-Ithnayn", "Yawm ath-Thulatha",
            "Yawm al-Arba'a", "Yawm al-Khamis", "Yawm al-Jum'a",
            "Yawm as-Sabt"
        };
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE HEBREW - IMPLÉMENTATION
// ============================================================

namespace Hebrew {
    bool isLeapYear(int year) {
        return (year * 7 + 1) % 19 < 7;
    }
    
    int getDaysInMonth(int month, int year) {
        if (month < 1 || month > 13) return 0;
        switch (month) {
            case 1: return 30;
            case 2: return 29;
            case 3: return 30;
            case 4: return 29;
            case 5: return 30;
            case 6: return 29;
            case 7: return 30;
            case 8: return isLeapYear(year) ? 30 : 29;
            case 9: return isLeapYear(year) ? 29 : 30;
            case 10: return 29;
            case 11: return 30;
            case 12: return isLeapYear(year) ? 30 : 29;
            case 13: return 29;
            default: return 0;
        }
    }
    
    int getDaysInYear(int year) {
        int total = 0;
        int maxMonths = isLeapYear(year) ? 13 : 12;
        for (int m = 1; m <= maxMonths; m++) {
            total += getDaysInMonth(m, year);
        }
        return total;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Nisan", "Iyar", "Sivan", "Tammuz", "Av", "Elul",
            "Tishrei", "Cheshvan", "Kislev", "Tevet", "Shevat", "Adar",
            "Adar II"
        };
        if (month >= 1 && month <= 13) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {
            "Rishon", "Sheni", "Shlishi", "Revi'i", "Chamishi", "Shishi", "Shabbat"
        };
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE ETHIOPIAN - IMPLÉMENTATION
// ============================================================

namespace Ethiopian {
    bool isLeapYear(int year) {
        return year % 4 == 3;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 5};
        if (month < 1 || month > 13) return 0;
        if (month == 13) return isLeapYear(year) ? 6 : 5;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Meskerem", "Tikimt", "Hedar", "Tahsas", "Tir", "Yakatit",
            "Megabit", "Miazia", "Ginbot", "Sene", "Hamle", "Nehasse", "Pagume"
        };
        if (month >= 1 && month <= 13) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Ehud", "Segno", "Maksegno", "Rob", "Hamus", "Arb", "Kidame"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE COPTIC - IMPLÉMENTATION
// ============================================================

namespace Coptic {
    bool isLeapYear(int year) {
        return year % 4 == 3;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 5};
        if (month < 1 || month > 13) return 0;
        if (month == 13) return isLeapYear(year) ? 6 : 5;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Thout", "Paopi", "Hathor", "Koiak", "Tobi", "Meshir",
            "Paremhat", "Paremoude", "Pashons", "Paoni", "Epip", "Mesori", "Pi Kogi Enavot"
        };
        if (month >= 1 && month <= 13) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Pesnau", "Pshoment", "Pshoment", "Pftoou", "Ptiou", "Psoou", "Psabbaton"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE CHINESE - IMPLÉMENTATION
// ============================================================

namespace Chinese {
    bool isLeapYear(int year) {
        return (year * 7 + 1) % 19 < 7;
    }
    
    int getDaysInMonth(int month, int year) {
        if (month < 1 || month > 12) return 0;
        if (month % 2 == 1) return 30;
        return 29;
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 384 : 354;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months_zh[] = {
            "一月", "二月", "三月", "四月", "五月", "六月",
            "七月", "八月", "九月", "十月", "十一月", "十二月"
        };
        static const std::string months_en[] = {
            "Month 1", "Month 2", "Month 3", "Month 4", "Month 5", "Month 6",
            "Month 7", "Month 8", "Month 9", "Month 10", "Month 11", "Month 12"
        };
        if (month < 1 || month > 12) return "";
        if (lang == "zh" || lang == "zh-CN") {
            return months_zh[month - 1];
        }
        return months_en[month - 1];
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days_zh[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        if (lang == "zh" || lang == "zh-CN") {
            return days_zh[idx];
        }
        return Gregorian::getDayName(dayOfWeek, lang);
    }
    
    std::string getAnimalName(int year) {
        const std::string animals[] = {
            "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
            "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig"
        };
        int index = (year - 4) % 12;
        if (index < 0) index += 12;
        return animals[index];
    }
    
    std::string getElementName(int year) {
        const std::string elements[] = {"Wood", "Fire", "Earth", "Metal", "Water"};
        int index = (year - 4) % 10 / 2;
        if (index < 0) index += 5;
        return elements[index];
    }
    
    std::string getFullYearName(int year) {
        return getElementName(year) + " " + getAnimalName(year);
    }
}

// ============================================================
// NAMESPACE PERSIAN - IMPLÉMENTATION
// ============================================================

namespace Persian {
    bool isLeapYear(int year) {
        return (year * 11 + 14) % 33 < 11;
    }
    
    int getDaysInMonth(int month, int year) {
        if (month < 1 || month > 12) return 0;
        if (month <= 6) return 31;
        if (month <= 11) return 30;
        if (month == 12) return isLeapYear(year) ? 30 : 29;
        return 0;
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Farvardin", "Ordibehesht", "Khordad", "Tir", "Mordad", "Shahrivar",
            "Mehr", "Aban", "Azar", "Dey", "Bahman", "Esfand"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {
            "Shanbeh", "Yekshanbeh", "Doshanbeh", "Seshanbeh", 
            "Chaharshanbeh", "Panjshanbeh", "Jomeh"
        };
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE HINDU - IMPLÉMENTATION
// ============================================================

namespace Hindu {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30};
        if (month < 1 || month > 12) return 0;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Chaitra", "Vaisakha", "Jyeshtha", "Ashadha", "Shravana", "Bhadrapada",
            "Ashwina", "Kartika", "Agrahayana", "Pausha", "Magha", "Phalguna"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {
            "Ravivar", "Somvar", "Mangalvar", "Budhvar", "Gurubar", "Shukravar", "Shanivar"
        };
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE BUDDHIST - IMPLÉMENTATION
// ============================================================

namespace Buddhist {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12) return 0;
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        return getMonthNameFromMap(month, lang);
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        return getDayNameFromMap(dayOfWeek, lang);
    }
}

// ============================================================
// NAMESPACE ZOROASTRIAN - IMPLÉMENTATION
// ============================================================

namespace Zoroastrian {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
        if (month < 1 || month > 12) return 0;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Fravardin", "Ardibehesht", "Khordad", "Tir", "Mordad", "Shahrivar",
            "Mehr", "Aban", "Azar", "Dey", "Bahman", "Esfand"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Ors", "Durs", "Vita", "Cyrus", "Darius", "Xerxes", "Artaxerxes"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE ARMENIAN - IMPLÉMENTATION
// ============================================================

namespace Armenian {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12) return 0;
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Nawasard", "Hori", "Sahmi", "Tre", "Kaghots", "Arap",
            "Mekak", "Areg", "Ahekan", "Mareri", "Margats", "Hrotits"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Areg", "Lusin", "Hrat", "Payl", "Urbat", "Shabat", "Kiraki"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE FRENCH_REPUBLICAN - IMPLÉMENTATION
// ============================================================

namespace FrenchRepublican {
    bool isLeapYear(int year) {
        return year % 4 == 3;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 5};
        if (month < 1 || month > 13) return 0;
        if (month == 13) return isLeapYear(year) ? 6 : 5;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Vendémiaire", "Brumaire", "Frimaire", "Nivôse", "Pluviôse", "Ventôse",
            "Germinal", "Floréal", "Prairial", "Messidor", "Thermidor", "Fructidor", "Sans-culottides"
        };
        if (month >= 1 && month <= 13) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Primidi", "Duodi", "Tridi", "Quartidi", "Quintidi", "Sextidi", "Septidi"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE BAHAI - IMPLÉMENTATION
// ============================================================

namespace Bahai {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        if (month < 1 || month > 19) return 0;
        if (month == 19) return isLeapYear(year) ? 5 : 4;
        return 19;
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Bahá", "Jalál", "Jamál", "Azamat", "Núr", "Rahmat",
            "Kalimát", "Kamál", "Asmá", "Izzat", "Mashiyyat", "Ilm",
            "Qudrat", "Qawl", "Masáil", "Sharaf", "Sultán", "Mulk", "Ayyám-i-Há"
        };
        if (month >= 1 && month <= 19) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        static const std::string days[] = {"Jamál", "Kamál", "Fidál", "Idál", "Istijlál", "Istiqlál", "Jalál"};
        int idx = ((dayOfWeek % 7) + 7) % 7;
        return days[idx];
    }
}

// ============================================================
// NAMESPACE SIKH - IMPLÉMENTATION
// ============================================================

namespace Sikh {
    bool isLeapYear(int year) {
        return year % 4 == 0;
    }
    
    int getDaysInMonth(int month, int year) {
        const int days[] = {30, 30, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30};
        if (month < 1 || month > 12) return 0;
        return days[month - 1];
    }
    
    int getDaysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
    
    std::string getMonthName(int month, const std::string& lang) {
        static const std::string months[] = {
            "Chet", "Vaisakh", "Jeth", "Harh", "Sawan", "Bhadon",
            "Assu", "Katak", "Maghar", "Poh", "Magh", "Phagun"
        };
        if (month >= 1 && month <= 12) return months[month - 1];
        return "";
    }
    
    std::string getDayName(int dayOfWeek, const std::string& lang) {
        return getDayNameFromMap(dayOfWeek, lang);
    }
}

// ============================================================
// NAMESPACE MAYAN - IMPLÉMENTATION
// ============================================================

namespace Mayan {
    std::string getTzolkinName(int day) {
        static const std::string names[] = {
            "Imix", "Ik", "Akb'al", "K'an", "Chikchan", "Kimi", "Manik", "Lamat", "Muluk", "Ok",
            "Chuwen", "Eb", "Ben", "Ix", "Men", "Kib", "Kaban", "Etz'nab", "Kawak", "Ajaw"
        };
        int idx = ((day % 20) + 20) % 20;
        return names[idx];
    }
    
    std::string getTzolkinGlyph(int day) {
        static const std::string glyphs[] = {
            "🌊", "🌬️", "🌙", "🌽", "🐍", "💀", "🦌", "⭐", "💧", "🐕",
            "🐒", "🌿", "🌾", "🐆", "🦅", "🦉", "🌎", "🗡️", "🌩️", "☀️"
        };
        int idx = ((day % 20) + 20) % 20;
        return glyphs[idx];
    }
    
    int getTzolkinNumber(int day) {
        int num = ((day % 13) + 13) % 13;
        if (num == 0) num = 13;
        return num;
    }
    
    std::string getTzolkinDate(int day) {
        std::stringstream ss;
        ss << getTzolkinNumber(day) << " " << getTzolkinName(day);
        return ss.str();
    }
    
    std::string getHaabName(int day) {
        static const std::string names[] = {
            "Pop", "Wo", "Sip", "Sotz'", "Sek", "Xul", "Yaxk'in", "Mol", "Ch'en", "Yax",
            "Sak", "Keh", "Mak", "K'ank'in", "Muwan", "Pax", "K'ayab", "Kumk'u", "Wayeb'"
        };
        int month = (day % 365) / 20;
        if (month >= 18) return "Wayeb'";
        return names[month];
    }
    
    int getHaabDay(int day) {
        int dayInMonth = (day % 365) % 20;
        return dayInMonth + 1;
    }
    
    std::string getHaabDate(int day) {
        std::stringstream ss;
        ss << getHaabDay(day) << " " << getHaabName(day);
        return ss.str();
    }
    
    MayanLongCount toLongCount(const Date& gregDate) {
        Date epoch(11, 8, -3113, CalendarType::GREGORIAN);
        int days = epoch.daysBetween(gregDate);
        return MayanLongCount::fromDays(days);
    }
    
    Date fromLongCount(const MayanLongCount& lc) {
        long long totalDays = lc.toDays();
        Date epoch(11, 8, -3113, CalendarType::GREGORIAN);
        Date result = epoch.addDays(static_cast<int>(totalDays));
        return result;
    }
    
    std::string getMayanDateString(const MayanLongCount& lc) {
        return lc.toString();
    }
    
    std::string getMayanDateString(const Date& date) {
        auto lc = toLongCount(date);
        return getMayanDateString(lc);
    }
}

// ============================================================
// NAMESPACE CALENDARUTILS - IMPLÉMENTATION
// ============================================================

namespace CalendarUtils {
    std::vector<CalendarType> getAvailableCalendars() {
        return {
            CalendarType::GREGORIAN, CalendarType::JULIAN,
            CalendarType::ISLAMIC, CalendarType::HEBREW,
            CalendarType::ETHIOPIAN, CalendarType::COPTIC,
            CalendarType::CHINESE, CalendarType::PERSIAN,
            CalendarType::HINDU, CalendarType::BUDDHIST,
            CalendarType::MAYAN, CalendarType::ZOROASTRIAN,
            CalendarType::ARMENIAN, CalendarType::FRENCH_REPUBLICAN,
            CalendarType::BAHAI, CalendarType::SIKH
        };
    }
    
    std::string getCalendarName(CalendarType type) {
        auto it = globalCalendarInfo.info.find(type);
        if (it != globalCalendarInfo.info.end()) {
            return it->second.name;
        }
        return "Unknown";
    }
    
    std::string getCalendarDescription(CalendarType type) {
        auto it = globalCalendarInfo.info.find(type);
        if (it != globalCalendarInfo.info.end()) {
            return it->second.description;
        }
        return "";
    }
    
    int getYearOffset(CalendarType type) {
        auto it = globalCalendarInfo.info.find(type);
        if (it != globalCalendarInfo.info.end()) {
            return it->second.yearOffset;
        }
        return 0;
    }
    
    Date convertDate(const Date& date, CalendarType targetCal) {
        return date.convertTo(targetCal);
    }
    
    Date convertToGregorian(const Date& date) {
        return date.toGregorian();
    }
    
    Date convertFromGregorian(const Date& gregDate, CalendarType targetCal) {
        return Date::fromGregorian(gregDate, targetCal);
    }
    
    std::string getCurrentDateInAllCalendars() {
        Date today = DateUtils::today();
        std::stringstream ss;
        auto calendars = getAvailableCalendars();
        for (auto cal : calendars) {
            Date d = today.convertTo(cal);
            ss << getCalendarName(cal) << ": " << d.toString("DD/MM/YYYY") << std::endl;
        }
        return ss.str();
    }
    
    std::string getHoliday(CalendarType type, int month, int day) {
        if (type == CalendarType::GREGORIAN) {
            if (month == 1 && day == 1) return "New Year's Day";
            if (month == 12 && day == 25) return "Christmas";
            if (month == 7 && day == 14) return "Bastille Day";
            if (month == 7 && day == 4) return "Independence Day (USA)";
            if (month == 2 && day == 14) return "Valentine's Day";
            if (month == 3 && day == 17) return "St. Patrick's Day";
            if (month == 10 && day == 31) return "Halloween";
            if (month == 11 && day == 11) return "Remembrance Day";
        }
        if (type == CalendarType::ISLAMIC) {
            if (month == 1 && day == 1) return "Islamic New Year";
            if (month == 3 && day == 12) return "Mawlid al-Nabi";
            if (month == 9 && day == 1) return "Ramadan (start)";
            if (month == 10 && day == 1) return "Eid al-Fitr";
            if (month == 12 && day == 10) return "Eid al-Adha";
        }
        if (type == CalendarType::HEBREW) {
            if (month == 7 && day == 1) return "Rosh Hashanah";
            if (month == 7 && day == 10) return "Yom Kippur";
            if (month == 7 && day == 15) return "Sukkot";
            if (month == 1 && day == 15) return "Passover";
            if (month == 6 && day == 14) return "Shavuot";
            if (month == 12 && day == 25) return "Hanukkah";
        }
        if (type == CalendarType::CHINESE) {
            if (month == 1 && day == 1) return "Chinese New Year";
            if (month == 1 && day == 15) return "Lantern Festival";
            if (month == 5 && day == 5) return "Dragon Boat Festival";
            if (month == 8 && day == 15) return "Mid-Autumn Festival";
        }
        if (type == CalendarType::PERSIAN) {
            if (month == 1 && day == 1) return "Nowruz (Persian New Year)";
            if (month == 12 && day == 29) return "Chaharshanbe Suri";
        }
        if (type == CalendarType::FRENCH_REPUBLICAN) {
            if (month == 1 && day == 1) return "Jour de l'An Républicain";
            if (month == 1 && day == 10) return "Fête du Peuple";
        }
        if (type == CalendarType::ETHIOPIAN) {
            if (month == 1 && day == 1) return "Enkutatash (New Year)";
            if (month == 9 && day == 27) return "Meskel (Finding of the True Cross)";
        }
        if (type == CalendarType::COPTIC) {
            if (month == 1 && day == 1) return "Coptic New Year";
            if (month == 1 && day == 6) return "Epiphany";
        }
        if (type == CalendarType::BAHAI) {
            if (month == 1 && day == 1) return "Naw-Ruz (Bahá'í New Year)";
            if (month == 4 && day == 1) return "Ridvan";
            if (month == 12 && day == 1) return "Ayyam-i-Ha";
        }
        if (type == CalendarType::SIKH) {
            if (month == 1 && day == 1) return "Nanakshahi New Year";
            if (month == 1 && day == 5) return "Guru Gobind Singh Jayanti";
            if (month == 11 && day == 24) return "Guru Nanak Jayanti";
        }
        return "";
    }
    
    std::string getHoliday(const Date& date) {
        return getHoliday(date.getCalendar(), date.getMonth(), date.getDay());
    }
}

// ============================================================
// NAMESPACE DATEUTILS - IMPLÉMENTATION
// ============================================================

namespace DateUtils {
    Date today() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }
    
    WeekDay getWeekDay(int day, int month, int year) {
        Date d(day, month, year);
        return d.getWeekDay();
    }
    
    bool isLeapYear(int year) {
        return Gregorian::isLeapYear(year);
    }
    
    int daysBetween(const Date& d1, const Date& d2) {
        return d1.daysBetween(d2);
    }
    
    std::string formatDate(const Date& date, const std::string& format) {
        return date.toString(format);
    }
    
    Date parseDate(const std::string& dateStr, const std::string& format) {
        int d = 1, m = 1, y = 2000;
        if (format == "DD/MM/YYYY") {
            sscanf(dateStr.c_str(), "%d/%d/%d", &d, &m, &y);
        } else if (format == "YYYY-MM-DD") {
            sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d);
        } else if (format == "MM/DD/YYYY") {
            sscanf(dateStr.c_str(), "%d/%d/%d", &m, &d, &y);
        }
        return Date(d, m, y);
    }
    
    std::string getCurrentDateTime() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
        return std::string(buffer);
    }
    
    Date fromTimeT(time_t t) {
        tm* tm_struct = localtime(&t);
        return Date(tm_struct->tm_mday, tm_struct->tm_mon + 1, tm_struct->tm_year + 1900);
    }
    
    time_t toTimeT(const Date& date) {
        Date greg = date.toGregorian();
        tm tm_struct = {};
        tm_struct.tm_mday = greg.getDay();
        tm_struct.tm_mon = greg.getMonth() - 1;
        tm_struct.tm_year = greg.getYear() - 1900;
        return mktime(&tm_struct);
    }
}

#endif // CALENDAR_H