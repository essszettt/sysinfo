# SYSINFO

Application to read system information on a ZX Soectrum Next. All information can be saved as text file.

The application is implemented as a DOT-command and can be called directly from BASIC.

---

## Usage

SYSINFO - System Info ZX Spectrum Next

    .SYSINFO file [-t rvo] [-f] [-q] [-h] [-v] 

    file        name of logfile
    -t[opic]    topics to show:
                r[eg] v[ar] o[s]
    -f[orce]    force overwrite
    -q[uiet]    no screen output
    -h[elp]     print this help
    -v[ersion]  print version info

---

## Examples

#### .sysinfo

Show all information on the screen

#### .sysinfo . -v

Show all information on the screen and save all in the current directory with a default name ("./sysinfo-0.txt")

#### .sysinfo /home/tmp/info.txt -t r

Show only registervalues on the screen and save it in with the given pathname

#### .sysinfo /home/tmp -t rv -q

Save registervalues and systemvariables with default name in the given directory without output on screen
