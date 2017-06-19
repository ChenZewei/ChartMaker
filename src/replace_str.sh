grep "OLDSTRING" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/OLDSTRING/NEWSTRING/g'

