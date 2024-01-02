help:
	@echo "Add translation		make add -T=jubl2000"
	@echo "Startup command		cd $(PWD); python3 server.py data/jubl2000"
	@echo "You may have to install flask + waitress: pip3 install flask"
	
add:
	@mkdir data
	@mkdir data/$(T)
	@cd data/$(T); wget https://api.heb12.com/translations/json/en/$(T).json
	@node compile.js ./data/$(T)/$(T).json data/$(T)

run:
	python3 server.py data/jubl2000
