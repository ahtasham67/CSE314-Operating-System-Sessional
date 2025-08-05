#!/bin/bash
mkdir -p "target/morning" "target/afternoon" "target/evening"

morningcnt=0
afternooncnt=0
eveningcnt=0

for photo in "photos_input"/IMG*.jpg; do
    filename=$(basename "$photo") 
    #echo "$filename" 
    hour1=$(echo "$filename" | cut -d '_' -f 3 | cut -d '.' -f 1)  
     hour=${hour1::2}
    if [ "$hour" -ge 0 ] && [ "$hour" -le 11 ]; then
        category="morning"
        targetdir="target/morning"
        ((morningcnt++))
    elif [ "$hour" -ge 12 ] && [ "$hour" -le 17 ]; then
        category="afternoon"
        targetdir="target/afternoon"
        ((afternooncnt++))
    else
        category="evening"
        targetdir="target/evening"
        ((eveningcnt++))
    fi
    newfile="$category $filename"
    
    cp "$photo" "$targetdir/$newfile"
done

echo "Morning: $morningcnt" > "counts.txt"
echo "Afternoon: $afternooncnt" >> "counts.txt"
echo "Evening: $eveningcnt" >> "counts.txt"
