#!/bin/sh

mkdir -p ~/.local/share/heb12
mkdir -p ~/.local/share/heb12/app
cd ~/.local/share/heb12/

curl -ipv4 https://api.heb12.com/translations/biblec/web.i > web.i
curl -ipv4 https://api.heb12.com/translations/biblec/web.t > web.t

echo "--- Will need superuser to write to /bin/heb12 ---"
if (uname -a | grep "x86_64"); then
	sudo curl -ipv4 -k http://api.heb12.com/static-heb12-x86_64 -o /bin/heb12
else
	echo "Sorry, only have x86_64 binary for now."
	echo "If you have git, gcc, and make, you can try the"
	echo "install build script: https://raw.githubusercontent.com/heb12/cli/master/install.sh"
fi
