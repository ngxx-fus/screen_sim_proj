# ABOUT

This project is made to develop the UI (from scratch). I realize that every time I want to see the changes, I have to wait for flashing... It takes a lot of my time!

# How does it work

## Directory tree

```
    ├── Makefile            # build script
    ├── README.md
    ├── app
    ├── app.c               # application logic (UI, ...)
    ├── app.o
    ├── config              # config of simulation
    │   ├── color.h
    │   └── config.h
    ├── include             # headers of simulation
    │   ├── app.h
    │   ├── global.h
    │   └── sim_api.h
    ├── leak_check.txt
    ├── lib                 # local lib for simulation
    │   ├── log
    │   │   ├── log.c
    │   │   ├── log.h
    │   │   └── log.o
    │   └── queue
    │       ├── queue.c
    │       ├── queue.h
    │       └── queue.o
    ├── src                 # The source code is being simulated
    │   ├── include
    │   │   └── main.h
    │   ├── main.c
    │   └── main.o
    └── valgrind.5989.log
```

**Note:**  
- `src/` here simulates the same structure as PlatformIO’s `src/`.  
  When running on PC, this directory contains the simulation entry (`main.c`).  
  When flashing to hardware, this will be replaced by the actual firmware `src/`.  
- `app/`, `lib/`, and `config/` are designed to be portable across both simulation and real firmware.  




