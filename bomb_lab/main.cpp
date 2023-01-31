#include "bomb_utils.h"

using namespace std;

int main() {
    char buf[BUF_SIZE];

    cout << "You have 6 phases with which to blow yourself up. Have a nice day!" << endl;
    // puts("You have 6 phases with which to blow yourself up. Have a nice day!\n");

    read_line(buf);
    phase_1(buf);
    cout << "Phase 1 defused. How about the next one?" << endl;

    read_line(buf);
    phase_2(buf);
    cout << "That's number 2.  Keep going!" << endl;

    read_line(buf);
    phase_3(buf);
    cout << "Halfway there!" << endl;

    int64_t ga, gb, gc;
    cin >> ga;
    phase_4(ga);
    cout << "So you got that one.  Try this one." << endl;

    cin >> ga >> gb >> gc;
    phase_5(ga, gb, gc);
    cout << "Good work!  On to the next..." << endl;

    cin >> buf;
    phase_6(buf);

    congratulations();
    return 0;
}