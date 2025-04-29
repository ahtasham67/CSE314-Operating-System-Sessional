#!/bin/bash
if [ "$#" -lt 4 ]; then
    echo "Required formate of arguments:"
    echo "Usage: $0 <submission_folder> <target_folder> <test_folder> <answer_folder> [options]"
    exit 1
fi
submissions="$1"
targets="$2"
tests="$3" 
answers="$4" 
shift 4
# Optional arguments
declare verbose=false
declare noexecute=false
declare nolc=false
declare nocc=false
declare nofc=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        -v)
            verbose=true
            ;;
        -noexecute)
            noexecute=true
            ;;
        -nolc)
            nolc=true
            ;;
        -nocc)
            nocc=true
            ;;
        -nofc)
            nofc=true
            ;;
        *)
            echo "Unknown option: $1"
            #exit 1
            ;;
    esac
    shift
done

report_head="student_id,student_name,language"
[ "$noexecute" = false ] && report_head+=",matched,not_matched"
[ "$nolc" = false ] && report_head+=",line_count"
[ "$nocc" = false ] && report_head+=",comment_count"
[ "$nofc" = false ] && report_head+=",function_count"
echo "$report_head" > result.csv

mkdir -p "$targets/C" "$targets/C++" "$targets/Java" "$targets/Python" 
for file in "$submissions"/*.zip
    do
        student_id=$(echo "$file" | cut -d'_' -f4 | cut -d'.' -f1)
        student_name=$(echo "$file" | cut -d'/' -f2 | cut -d'_' -f1)
        #echo "$student_name,$student_id"
        if [ "$verbose" = true ]; then
            echo "Organizing files of $student_id"
        fi
        rm -rf temp > /dev/null
        mkdir temp > /dev/null
        unzip -d temp "$file" > /dev/null

        code_file=""
        for ext in c cpp py java; do
            code_file=$(find temp -type f -name "*.$ext" | head -n 1) > /dev/null
            if [ -n "$code_file" ]
            then break
            fi
        done

       #echo "${code_file##*.}"
       case "${code_file##*.}" in
            c)
                lang_dir="C"
                ext="c"
                language="C"
            ;;
            cpp)
                lang_dir="C++"
                ext="cpp"
                language="C++"
            ;;
            py)
                lang_dir="Python"
                ext="py"
                language="Python"
            ;;
            java)
                lang_dir="Java"
                ext="java"
                language="Java"
            ;;
            *)
                echo "Unsupported file type for student ID: $student_id"
                rm -rf temp
                continue
            ;;
        esac

       path="$targets/$lang_dir/$student_id"
        mkdir -p $path

        not_matched=0
        match_count=0

        case "$ext" in
            c|cpp)
                cp "$code_file" "$path/main.$ext";
                if [[ "$nolc" = false ]] ; then
                    lineNum=$(wc -l < "$path/main.$ext" | xargs);
                fi
                if [[ "$nocc" = false ]] ; then
                    commentNum=$(grep -c "//" $path/main.$ext);
                fi
                if [[ "$nofc" = false ]] ; then
                    functionNum=$(grep -cE "\\b(void|int|float|double|char|bool)\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\(" "$path/main.$ext")
                fi  
                #echo "Function count: $functionNum"
                if [[ "$noexecute" = false ]] ; then
                echo "Executing files of $student_id"
                g++ -o $path/main $path/main.$ext;
                chmod +x $path/main;
                 for test_file in "$tests"/test*.txt
                    do 
                    test_name=$(basename "$test_file" .txt)
                    test_num="${test_file//[^0-9]/}"
                    $path/main < "$test_file" > "$path/out$test_num.txt"
                    
                    if diff -q "$path/out$test_num.txt" "$answers/ans$test_num.txt" > /dev/null; then
                        ((match_count++))
                    else
                        ((not_matched++))
                    fi
                    done
                fi 
                ;;
            py)
                cp "$code_file" "$path/main.$ext";
                if [[ "$nolc" = false ]] ; then
                    lineNum=$(wc -l < "$path/main.$ext" | xargs);
                fi
                if [[ "$nocc" = false ]] ; then
                    commentNum=$(grep -c "#" $path/main.$ext);
                fi
                if [[ "$nofc" = false ]] ; then
                    functionNum=$(grep -cE "\\b(def)\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\(" "$path/main.$ext")
                fi
                
                if [[ "$noexecute" = false ]] ; then
                echo "Executing files of $student_id"
                chmod +x "$path/main.$ext"
                 for test_file in "$tests"/test*.txt
                    do 
                    test_name=$(basename "$test_file" .txt)
                    test_num="${test_file//[^0-9]/}"
                    python3 $path/main.py < "$test_file" > "$path/out$test_num.txt"
                    if diff -q "$path/out$test_num.txt" "$answers/ans$test_num.txt" > /dev/null; then
                        ((match_count++))
                    else
                        ((not_matched++))
                    fi
                    done 
                fi
                ;;
            java)
            cp "$code_file" "$path/Main.$ext"
            if [[ "$nolc" = false ]] ; then
                lineNum=$(wc -l < "$path/Main.$ext" | xargs);
            fi
            if [[ "$nocc" = false ]] ; then
                commentNum=$(grep -c "//" $path/Main.$ext);
            fi
            if [[ "$nofc" = false ]] ; then
                functionNum=$(grep -cE "^\s*(public|private|protected)?\s*(static\s+)?(void|int|float|double|char|boolean|String)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\(" "$path/Main.$ext")
            fi   
            if [[ "$noexecute" = false ]] ; then
            echo "Executing files of $student_id"
            javac -d "$path" "$path/Main.$ext"
            for test_file in $tests/test*.txt; do
                test_name=$(basename "$test_file" .txt)
                test_num="${test_name//[^0-9]/}"
                (cd "$path" && java Main < "../../../$test_file" > "out${test_num}.txt")
                if diff -q "$path/out$test_num.txt" "$answers/ans$test_num.txt" > /dev/null; then
                    ((match_count++))
                else
                    ((not_matched++))
                fi
                done
                fi
                ;;
        esac
        rm -rf temp > /dev/null  
    csv_entry="$student_id,\"$student_name\",$language"
    [ "$noexecute" = false ] && csv_entry+=",$match_count,$not_matched"
    [ "$nolc" = false ] && csv_entry+=",$lineNum"
    [ "$nocc" = false ] && csv_entry+=",$commentNum"
    [ "$nofc" = false ] && csv_entry+=",$functionNum"
    echo "$csv_entry" >> result.csv
    done 
    if [ "$verbose" = true ]; then
    echo "All submissions processed successfully" 
    fi 