#include <stdio.h>
#include <sys/termios.h>

int main(int argc, const char * argv[]) {
    printf("Saving config\n");
    struct termios saved_tio;
    struct termios tio;
    if (tcgetattr(fileno(stdin), &tio) == -1) {
        printf("  Could not get config\n");
        return -1;
    }
    saved_tio = tio;
    
    printf("Entering raw mode\n");
    tio.c_iflag |= IGNPAR;
    tio.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXANY | IXOFF);
#ifdef IUCLC
    tio.c_iflag &= ~IUCLC;
#endif
    tio.c_lflag &= ~(ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL);
#ifdef IEXTEN
    tio.c_lflag &= ~IEXTEN;
#endif
    tio.c_oflag &= ~OPOST;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(fileno(stdin), TCSADRAIN, &tio) == -1) {
        printf("  Could not set config\n");
        return -1;
    }
    
    printf("Restoring config\n");
    if (tcsetattr(fileno(stdin), TCSADRAIN, &saved_tio) == -1) {
        printf("  Could not restore config\n");
    }
    return 0;
}
