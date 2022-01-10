#!/bin/sh

echo "Making folders"
mkdir -p ~/.local/share/heb12
mkdir -p ~/.local/share/heb12/app
cd ~/.local/share/heb12/

echo "Downloading Web (biblec)"
curl -L https://api.heb12.com/translations/biblec/web.i > web.i
curl -L https://api.heb12.com/translations/biblec/web.t > web.t

echo "--- Will need superuser to write to /bin/heb12 ---"
if (uname -a | grep "x86_64"); then
	sudo curl -L "https://github.com/heb12/cli/releases/download/0.1.2/static-heb12-x86_64" -o /bin/heb12
	echo "/bin/heb12 Installed"
else
	echo "Sorry, only have x86_64 binary for now."
	echo "If you have git, gcc, and make, you can try the"
	echo "install build script:"
	echo "curl -L https://raw.githubusercontent.com/heb12/cli/master/install.sh | sh"
fi
