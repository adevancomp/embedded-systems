## PRECONDITION

### The Dockerfile must be in the same directory as your Zephyr project and STM32CubeProgrammer folder. The SDK directory must be inside the Zephyr project, and its name should be just sdk.

## Change path to your folders

```bash
export MY_ZEPHYR_PROJECT=/home/[your user]/zephyrproject
export MY_FIRMWARE_PROJECT=/home/[your user]/[project name]
```

tree -L 2
```bash
.
├── Dockerfile.zephyr
├── install-docker.sh
├── STM32CubeProgrammer
│   ├── api
│   ├── bin
│   ├── Data_Base
│   ├── doc
│   ├── Drivers
│   ├── IzPackLocaleEnabledXdgDesktopIconScript.sh
│   ├── lib
│   ├── SVD
│   ├── uninstaller
│   └── util
└── zephyrproject
    ├── bootloader
    ├── modules
    ├── sdk
    ├── tools
    └── zephyr
```

## Instructions

```bash
./install-docker.sh
```
## Restart Machine

```bash
docker build -f Dockerfile.zephyr --build-arg MY_ZEPHYR_PROJECT_ARG=$MY_ZEPHYR_PROJECT -t custom-zephyr:1.0.0 .
```

```bash
docker run -it --name custom-zephyr-builder --privileged -v /dev/bus/usb:/dev/bus/usb -v $MY_FIRMWARE_PROJECT:/fmw custom-zephyr:1.0.0   
```

## Usage 

```bash
docker start -ai custom-zephyr-builder
```

### Obs: Use the same west and Zephyr commands. 
### Ex : west build, west flash. Output 

## Share docker image

```bash
docker save -o custom-zephyr_1.0.0.tar custom-zephyr:1.0.0
```

## Machine 2

```bash
docker load -i custom-zephyr_1.0.0.tar
```

## Docker images

```bash
  REPOSITORY     TAG      IMAGE ID       CREATED        SIZE
custom-zephyr   1.0.0   1dcl7dfb1ce4   1 hours ago     18.5GB
```
