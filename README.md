# DIBI - Did I Break It?

A lightweight command-line tool to quickly check if system services are running on Linux systems.

## Description

`did-i-break-it` (DIBI) is a simple utility that checks the status of systemd services and reports whether they're active, inactive, or encountering issues. Perfect for quick system health checks after configuration changes or system updates.
It will create a `.DIBI` directory in your $HOME directory, as well as a snapshopts folder. Config files will come soon, as well as a better CLI experience.

## Features

- ✅ Quick service status checks via systemctl
- ✅ Clear, human-readable output
- ✅ Handles common service states (active, inactive, unknown)
- ✅ Lightweight with minimal dependencies
- ✅ No elevated permissions needed for most checks

## Installation

### From AUR (Arch Linux)

```bash
yay -S did-i-break-it
# or
paru -S did-i-break-it
```

### Manual Installation

#### Prerequisites
- GCC compiler
- Make
- systemd

#### Build and Install

```bash
# Clone the repository
git clone https://github.com/NikzoOff/DIBI.git
cd DIBI

# Compile
make

# Install (requires sudo)
sudo make install
```

The binary will be installed to `/usr/local/bin/did-i-break-it`.

## Usage

```bash
did-i-break-it
```

### Output Examples

```bash
$ did-i-break-it
apache2 is not running (inactive/stopped)
NetworkManager is running and active
...
# + Check the .DIBI folder for detailet snapshots
```

## Exit Codes

The tool returns standard systemctl exit codes:
- `0` - Service is active and running
- `3` - Service is inactive/stopped
- `4` - Service status is unknown or doesn't exist
- `127` - systemctl command not found

## Building from Source

```bash
# Build only
make

# Test without installing
./did-i-break-it

# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild
```

## Uninstallation

### If installed from AUR
```bash
yay -R did-i-break-it
# or
paru -R did-i-break-it
```

### If installed manually
```bash
sudo make uninstall
```

## Development

### Project Structure
```
DIBI/
├── dibi.c          # Main source file
├── Makefile        # Build configuration
├── LICENSE         # MIT License
└── README.md       # This file
```

### Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests

Please ensure your code follows the existing style and includes appropriate documentation.

## Requirements

- **Operating System:** Linux with systemd
- **Compiler:** GCC or compatible C compiler
- **Dependencies:** 
  - glibc
  - systemd

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

**NikzoOff**
- GitHub: [@NikzoOff](https://github.com/NikzoOff)

## Acknowledgments

Built as a simple utility for quick system service checks. Perfect for sysadmins, developers, and anyone who frequently asks themselves "did I break it?"

## Version History

- **v0.1.0** (Initial Release)
  - Basic service status checking
  - Support for systemctl integration
  - Clear exit codes for scripting

## Support

If you encounter any issues or have questions:
- Open an issue on [GitHub](https://github.com/NikzoOff/DIBI/issues)
- Check existing issues for solutions

---

**Note:** This tool requires systemd to be present on your system. It will not work on non-systemd distributions.
