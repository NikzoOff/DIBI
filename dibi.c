// Libraries needed.
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>

// --------------------
// Functions
int mkdirDIBI() {
  const char *homeDir = getenv("HOME");
  char dibiDir[256];
  char snapshotDir[512];
  snprintf(dibiDir, sizeof(dibiDir), "%s/.DIBI", homeDir);
  snprintf(snapshotDir, sizeof(snapshotDir), "%s/snapshots", dibiDir);

  if (mkdir(dibiDir, 0755) == -1) {
        if (errno == EEXIST) {
            printf(".DIBI directory already exists, skipping creation.\n");
        } else {
            perror("mkdir failed");
            exit(1);
        }
    } else {
      printf(".DIBI directory created successfully.\n");
  }

  if (mkdir(snapshotDir, 0755) == -1) {
        if (errno == EEXIST) {
            printf("Snapshots directory already exists, skipping creation.\n");
        } else {
            perror("mkdir failed");
            exit(1);
        }
    } else {
      printf("Snapshots directory created successfully.\n");
  }

  return 0;
}

int serviceTest(const char *serviceName, int isUserService) {
  char command[256];

  if (isUserService) {
    snprintf(command, sizeof(command), "systemctl --user is-active --quiet %s", serviceName);
  } else {
    snprintf(command, sizeof(command), "systemctl is-active --quiet %s", serviceName);
  } 

  int status = system(command);

  if (status == -1) {
      printf("Failed to execute shell.\n");
      return -1;
   }

  int exitCode = WEXITSTATUS(status);

  if (exitCode == 0) {
    printf("%s is running and active.\n", serviceName);
  } else if (exitCode == 3) {
    printf("%s is not running. (inactive/stopped)\n", serviceName);
  } else if (exitCode == 4) {
    printf("%s status is unknown (service may not exist).\n", serviceName); 
  } else if (exitCode == 127) {
    printf("Command 'systemctl' not found.\n");
  } else { 
    printf("%s could not be checked (exit code %d).\n", serviceName, exitCode);
  }

  return exitCode;
}

int runCmdToFile(const char *cmd, FILE *file) {
  FILE *fp;
  char buffer[256];

  fp = popen(cmd, "r");
  if (fp == NULL) {
      fprintf(stderr, "Failed to run command: %s\n", cmd);
      return 1;
  }

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      fprintf(file, "%s", buffer);
  }
  fprintf(file, "\n");
  pclose(fp);
  
  return 0;
}

void runCmdToConsole(const char *command) {
    char buffer[256];
    FILE *fp = popen(command, "r");

    if (fp == NULL) {
        printf("Failed to run command: %s\n", command);
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    pclose(fp);
}


// --------------------
// Main program

int main() {
  // 1. Create the .DIBI directory
  if (mkdirDIBI() != 0) {
    printf("The .DIBI directory failed to be created, exitting.\nTry checking if your /home/$USER exists. Or check if you have permissions to create it.\n");
    exit(1);
  }
  // 2. Defines timestamp and create file 
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  const char *homeDir = getenv("HOME");
  char snapshotFile[256];
  snprintf(snapshotFile, sizeof(snapshotFile), "%s/.DIBI/snapshots/%02d-%02d-%02d.txt",
           homeDir, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // 3. Open file in append mode
  FILE *file = fopen(snapshotFile, "a");
  if (!file) {
    perror("fopen failed");
    exit(1);
  }

  printf("The tests will begin in:\n");

    for (int i = 3; i > 0; i--) {
        printf("%d...\n", i);
        fflush(stdout);  // ensure it prints immediately
        sleep(1);        // wait 1 second
    }

  // 4. Write timestamp
  fprintf(file, "\nFile created automatically on %04d-%02d-%02d at %02d:%02d:%02d\n \n",
          tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
          tm.tm_hour, tm.tm_min, tm.tm_sec);

  // 5. System checks here
  
  printf("====== RUNNING THE FIRMWARE TESTS =======\n");
  // Network tests
  int networkManagerStatus = serviceTest("NetworkManager", 0);

  if (networkManagerStatus == 0) {
    fprintf(file, "- NetworkManager is running and operationnal. Check the command 'ping' to see if you are detect wifi and can resolve hostnames:\n");
    fprintf(file, "-----------------------------------\n \n");
  } else {
    fprintf(file, "NetworkManager is not running, resulting in no wifi on your machine.\n");
  }

  runCmdToFile("ping -c 1 8.8.8.8", file);
  runCmdToFile("ping -c 1 archwiki.org", file);
  runCmdToFile("nmcli device status", file);
  
  // Audio test
  int pipewireStatus = serviceTest("pipewire", 1);

  if (pipewireStatus == 0) {
    fprintf(file, "\n- PipeWire is running and operational.\n");
    fprintf(file, "-----------------------------------\n \n");
  } else if (pipewireStatus == 4) { // not found
    fprintf(file, "\nPipeWire hasn't been found, will test for PulseAudio.\n");
    fprintf(file, "-----------------------------------\n \n");

    int pulseStatus = serviceTest("pulseaudio", 1);

    if (pulseStatus == 0) {
      fprintf(file, "\n- PulseAudio is running and operational.\n");
      fprintf(file, "-----------------------------------\n \n");
    } else if (pulseStatus == 4) {
      fprintf(file, "\n- PulseAudio hasn't been found either. No audio server is running.\n");
      fprintf(file, "-----------------------------------\n \n");
    } else {
      fprintf(file, "\n- PulseAudio could not be checked (exit code %d).\n", pulseStatus);
      fprintf(file, "-----------------------------------\n \n");
    }
  } else {
    fprintf(file, "\n- PipeWire could not be checked (exit code %d).\n", pipewireStatus);
    fprintf(file, "-----------------------------------\n \n");
  }

  // DBUS tests
  int dbusStatus = serviceTest("dbus", 0);

  if (networkManagerStatus == 0) {
    fprintf(file, "\n- DBus system is running and operationnal.\n");
    fprintf(file, "-----------------------------------\n \n");
  } else {
    fprintf(file, "\nDbus could not be checked (exit code %d).\n", dbusStatus);
    fprintf(file, "-----------------------------------\n \n");
  }

  //LoginD tests
  int logindStatus = serviceTest("systemd-logind", 0);

  if (networkManagerStatus == 0) {
    fprintf(file, "\n- Systemd-logind system is running and operationnal.\n");
    fprintf(file, "-----------------------------------\n \n");
  } else {
    fprintf(file, "\nSystemd-logind could not be checked (exit code %d).\n", logindStatus);
    fprintf(file, "-----------------------------------\n \n");
  }

  // SSH tests
  int sshdStatus = serviceTest("sshd", 0);

  if (sshdStatus == 0) {
    fprintf(file, "\n- The SSHD server is running and operationnal.\n");
    fprintf(file, "-----------------------------------\n \n");
  } else {
    fprintf(file, "\nSSHD could not be checked (exit code %d).\n", sshdStatus);
    fprintf(file, "-----------------------------------\n \n");
  }

  runCmdToFile("sshd -T 2>&1", file);

  // Bluetooth tests
  int bluetoothStatus = serviceTest("bluetooth", 0);

  if (bluetoothStatus == 0) {
    fprintf(file, "\n- Bluetooth Manager is running and operationnal.\n");
    fprintf(file, "-----------------------------------\n \n");
  } else {
    fprintf(file, "\nBluetooth Manager could not be checked (exit code %d).\n", bluetoothStatus);
    fprintf(file, "-----------------------------------\n \n");
  }

  runCmdToFile("bluetoothctl list", file);

  printf("\n====== System Environment Infos ======\n");

  printf("Kernel: ");
  runCmdToConsole("uname -r");

  printf("Uptime: ");
  runCmdToConsole("uptime -p");

  printf("XDG SESSION type: ");
  runCmdToConsole("echo $XDG_SESSION_TYPE");

  printf("Shell: ");
  runCmdToConsole("echo $SHELL");

  printf("Disks: ");
  runCmdToConsole("df -h --output=used,size,pcent,target / | tail -1");
  fprintf(file, "\n- Disk Space used:\n");
  fprintf(file, "-----------------------------------\n \n");
  runCmdToFile("df -h /", file);

  fclose(file);

  return 0;
}
