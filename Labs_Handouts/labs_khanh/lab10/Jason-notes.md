# Lab 10 - CUPS
Marking script is running, will inform students shortly

For your notes, what needs to be done:

## CUPS Backend
- Students need to write script to implement pdf backend
- Students should not use cups-pdf port/package, this is too hard to configure, easier to just write script
- Backend needs to be installed as: /usr/local/libexec/cups/backend/pdf
- Permissions need to be set otherwise it won't run properly (rwx------)

## Configure CUPS
- Change Listen localhost:631 ==> Port 631
- Add ACL for Swinburne LAN
	- Add "Allow from 136.186.*.*" after "Order allow,deny" to the following:
	- # Restrict access to the server...
	- # Restrict access to the admin pages...
	- Might want to also add 10. to allow web access/config access from home via VPN

## Allow CUPS to auto start
	- Add cupsd_enable="YES" to /etc/rc.conf

## Install cups-filters package
	- pkg install cups-filters

## Start CUPS
	- /usr/local/etc/rc.d/cupsd start

## Install Printer
	- lpadmin -p PDFPrinter -D "PDF Printer for Lab 10" -E -L "RULE Host 20" -m lsb/local/cupsfilters/Generic-PDF_Printer-PDF.ppd -v pdf://

These are all for your notes, not to be given to students.
	1. For the backend you can/should advise that cups-pdf is not a good approach, direct them to the man pages, mention that file permissions can be important but that they need to research the problem
	2. For CUPS you can/should advise that running via the VPN may mean that they need to temporarily change the ACLs to allow access from home
	3. The Generic PDF printer driver is contained within the cups-filters package. Can tell students they need to install this
	4. This is a D tasks and supposed to separate D/HD from P/C, so don't guide them too much
	
# My Notes
Okay so here are steps I took myself for the Pass task:
- Install cups-filters package
	- pkg install cups-filters
	- If not installed, you will get the error "lpadmin: Unable to copy PPD file." when installing printer with `lpadmin` command
- Edit /usr/local/etc/cups/cupsd.conf
	- Listen 0.0.0.0:631
	- Allow 136.186.0.0/16 10.0.0.0/8 for / and /admin Location
- Edit /etc/rc.conf
	- cupsd_enable="YES"
- Start CUPS
	- service cupsd start
- Create an empty pdf backend in /usr/local/libexec/cups/backend and set the permission to 700. If this backend does not exist or the permission is not right, you will not be able to install the printer in the next step and will get the error Bad device-uri scheme "pdf".
	- touch pdf
	- chmod 700 pdf
- Install Printer. You can either do this by using lpadmin command or use the Web Interface. 
	- lpadmin -p PDFPrinter -D "PDF Printer for Lab 10" -E -L "RULE Host 20" -m lsb/local/cupsfilters/Generic-PDF_Printer-PDF.ppd -v pdf://
	- On web browser and navigate to 136.186.230.xx:631/admin and use the web interface to Add Printer

If students do not intend to do the Distinction task, they may get away with skipping step 5 by not using the pdf backend when installing the printer (i.e. remove the -v pdf:// option if using lpadmin command or just select any network printers available on the web page). The marking script for Pass task will only check if the PDFPrinter exists and doesn't care about which backend is being used by the printer.