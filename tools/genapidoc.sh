#!/bin/sh

cd $(dirname $0)

if which apigen &>/dev/null
then
	version="0.0.4"
	apigen generate \
		--todo \
		--deprecated \
		--tree \
		--title "PGGI version $version API" \
		--source ../doc \
		--destination ../doc/api \
		--php
else
	echo -e "\nApiGen not found, see http://www.apigen.org/\n"

fi
