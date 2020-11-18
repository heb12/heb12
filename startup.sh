#!/bin/bash
# Run this on start
mkdir bibles
cd bibles
wget https://api.heb12.com/translations/biblec/web.i
wget https://api.heb12.com/translations/biblec/web.t
cd ..
cc biblec/biblec.c fbrp/fbrp.c app.c
