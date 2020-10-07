# Run this on start
mkdir bibles
wget http://api.heb12.com/translations/json/en/web.json
node biblec/compiler.js ../web.json web bibles
cc biblec/biblec.c fbrp/fbrp.c app.c
