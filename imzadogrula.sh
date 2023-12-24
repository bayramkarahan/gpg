#!/bin/bash
file=$1
if [ $file == "" ]
then
echo "Dosya belirtiniz."
else
	gpg --verify "$file"
	status=$?
	 if [ "$status" == "0" ]
		then
	  		echo "İmza İyi"
	  	else
	  		echo "İmza Kötü"
	  fi 
fi
