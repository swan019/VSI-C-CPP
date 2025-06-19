
#include <stdio.h>
#include <errno.h>
#include <string.h>
//
//int main() {
//    FILE* fp = fopen("Game.txt", "r");
//
//    // Simulate normal output
//    printf("Program started successfully.\n");
//
//    // Simulate error
//    if (!fp) {
//        fprintf(stderr, "Error: Cannot open file '%s' [%d] %s\n",
//            "Game.txt", errno, strerror(errno));
//    }
//    else {
//        fclose(fp);
//    }
//
//    printf("Program ended normally.\n");
//
//    return 0;
//}



//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <errno.h> 
//
//
//int  main() {
//	FILE* pFile = NULL;
//	errno_t isErrAtFileOpening = fopen_s(&pFile, "Game.txt", "r");
//	if (isErrAtFileOpening != 0) {
//		printf("Message : %s\n ",strerror(isErrAtFileOpening));
//		printf("Error : %d\n", isErrAtFileOpening);
//	}
//	else {
//		fclose(pFile);
//	}
//	return 0;
//}


int main() {
	FILE* file = NULL;

	errno_t err = fopen_s(&file, "Game.txt", "r");
	if (err != 0) {
		char errorMsgBuffer[256];
		strerror_s(errorMsgBuffer, sizeof(errorMsgBuffer), err);
		fprintf_s(stderr, "Error : %s [%d]\n", errorMsgBuffer, err);
	}

}