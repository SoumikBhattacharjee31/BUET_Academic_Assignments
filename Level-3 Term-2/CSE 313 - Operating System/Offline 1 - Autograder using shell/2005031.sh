#!/usr/bin/bash

# argument count check
if [ $# -lt 0 ] || [ $# -gt 2 ]; then
    echo "Wrong number of arguments"
    exit 1
fi

# check -i
if [[ $1 != "-i" ]]; then
    echo "Invalid command"
    exit 1
fi

# get filename
filename=$2

# check file exists or not
if [ ! -f $filename ]; then
    echo "File does not exist"
    exit 1
fi

# make array of lines from file
mapfile lines < $filename
for i in "${!lines[@]}"; do
    line=${lines[$i]}
    line=$(echo "$line" | tr -d '\n' | tr -d '\r' | tr -d '\t')
    line="${line%% }"
    line="${line# }"
    lines[$i]=$line
done

# linecount check
if [ ${#lines[@]} -ne 11 ]; then
    echo "Invalid format: Number of lines are not matching"
    exit 1
fi

# first line check
if [[ "${lines[0]}" != "true" && "${lines[0]}" != "false" ]]; then
    echo "Invalid format: First line should contain either true of false"
    exit 1
fi

# second line check
tokens=( ${lines[1]} )
for token in ${tokens[@]}
do
    if [[ "$token" != "zip" && "$token" != "rar" && "$token" != "tar" ]]; then
        echo "Invalid format: Second line should contain zip, rar and tar"
        exit 1
    fi
done

# third line check
tokens=( ${lines[2]} )
if [ ${#tokens} -eq 0 ]; then
    echo "Invalid format: No valid language can be used"
    exit 1
fi
for token in ${tokens[@]}
do
    if [[ "$token" != "c" && "$token" != "cpp" && "$token" != "python" && "$token" != "sh" ]]; then
        echo "Invalid format: Third line should contain c, cpp, python and sh"
        exit 1
    fi
done

# fourth line check
re="^[0-9]+$"
if ! [[ ${lines[3]} =~ $re ]]; then
    echo "Invalid format: Fourth line should contain a number"
    exit 1
fi

# fifth line check
re="^[0-9]+$"
if ! [[ ${lines[4]} =~ $re ]]; then
    echo "Invalid format: Fifth line should contain a number"
    exit 1
fi

# sixth line check
if [ ${#lines[5]} -le 0 ]; then
    echo "Invalid format: Sixth line does not contain a directory name"
    exit 1
fi
if [ ${lines[5]:0:1} = "/" ]; then
    lines[5]=".${lines[5]}"
elif [ ${lines[5]:0:1} != "." ]; then
    lines[5]="./${lines[5]}"
fi
if [ ! -d "$(echo "${lines[5]}")" ]; then
    echo "Invalid format: Sixth line should contain an existing directory path"
    exit 1
fi

# seventh line check
tokens=( ${lines[6]} )
if [ ${#tokens[@]} -ne 2 ]; then
    echo "Invalid format: Seventh line must contain two student ids"
    exit 1
fi
re="^[0-9]+$"
if ! [[ ${tokens[0]} =~ $re ]]; then
    echo "Invalid format: Seventh line should contain two numbers"
    exit 1
fi
re="^[0-9]+$"
if ! [[ ${tokens[1]} =~ $re ]]; then
    echo "Invalid format: Seventh line should contain two numbers"
    exit 1
fi
if [ ${tokens[0]} -gt ${tokens[1]} ]; then
    echo "Invalid format: Seventh line must contain greater id at first"
    exit 1
fi

# eighth line check
if [ ${#lines[7]} -le 0 ]; then
    echo "Invalid format: Eighth line does not contain a file name"
    exit 1
fi
if [ ${lines[7]:0:1} = "/" ]; then
    lines[7]=".${lines[7]}"
elif [ ${lines[5]:0:1} != "." ]; then
    lines[7]="./${lines[7]}"
fi
if [ ! -f "$(echo "${lines[7]}")" ]; then
    echo "Invalid format: Eighth line should contain an existing file path"
    exit 1
fi

# ninth line check
re="^[0-9]+$"
if ! [[ ${lines[8]} =~ $re ]]; then
    echo "Invalid format: Ninth line should contain a number"
    exit 1
fi

# tenth line check
if [ ${#lines[9]} -le 0 ]; then
    echo "Invalid format: Tenth line does not contain a file name"
    exit 1
fi
if [ ${lines[9]:0:1} = "/" ]; then
    lines[9]=".${lines[9]}"
elif [ ${lines[9]:0:1} != "." ]; then
    lines[9]="./${lines[9]}"
fi
if [ ! -f "$(echo "${lines[9]}")" ]; then
    echo "Invalid format: Tenth line should contain an existing file path"
    exit 1
fi

# eleventh line check
re="^[0-9]+$"
if ! [[ "${lines[10]}" =~ $re ]]; then
    echo "Invalid format: Eleventh line should contain a number"
    exit 1
fi






# store lines to variables
USE_ARCHIVE=${lines[0]}
ALLOWED_ARCHIVED_FORMAT=( ${lines[1]} )
ALLOWED_PROGRAMMING_LANGUAGES=( ${lines[2]} )
TOTAL_MARKS=${lines[3]}
PENALTY_FOR_UNMATCHED_OUTPUT=${lines[4]}
WORKING_DIRECTORY=${lines[5]}
STUDENT_ID_RANGE=${lines[6]}
EXPECTED_OUTPUT_FILE_LOCATION=${lines[7]}
PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION=${lines[8]}
PLAGIARISM_ANALYSIS_FILE=${lines[9]}
PLAGIARISM_PENALTY=${lines[10]}

# fix language extension
for i in "${!ALLOWED_PROGRAMMING_LANGUAGES[@]}"; do
    if [[ ${ALLOWED_PROGRAMMING_LANGUAGES[$i]} = "python" ]]; then
        ALLOWED_PROGRAMMING_LANGUAGES[$i]="py"
    fi
done

ALLOWED_EXTENSIONS=("${ALLOWED_ARCHIVED_FORMAT[@]} ${ALLOWED_PROGRAMMING_LANGUAGES[@]}")

# make array of expected output line
mapfile expected_lines < $EXPECTED_OUTPUT_FILE_LOCATION
for i in "${!expected_lines[@]}"; do
    line=${expected_lines[$i]}
    line=$(echo "$line" | tr -d '\n' | tr -d '\r' | tr -d '\t')
    line="${line%% }"
    line="${line# }"
    expected_lines[$i]=$line
done

# make array of plagiarized id
mapfile plag_ids < $PLAGIARISM_ANALYSIS_FILE
for i in "${!plag_ids[@]}"; do
    line=${plag_ids[$i]}
    line=$(echo "$line" | tr -d '\n' | tr -d '\r' | tr -d '\t')
    line="${line%% }"
    line="${line# }"
    plag_ids[$i]=$line
done

# init
echo "id,marks,marks_deducted,total_marks,remarks" > marks.csv
if [ -e issues ]; then
    rm -r issues
fi
if [ -e checked ]; then
    rm -r checked
fi
mkdir issues
mkdir checked


# read filepaths
filepaths=()
while IFS= read -r -d $'\0' item; do
  filepaths+=("$item")
done < <(find "$WORKING_DIRECTORY" -maxdepth 1 -mindepth 1 -print0)


prev_id=( $STUDENT_ID_RANGE )
prev_id=${prev_id[0]}

# evaluation
for path in ${filepaths[@]}; do
    name=$(basename "$path")
    id=0
    case_1=0

    # file
    if [[ -f $path ]]; then
        # check filename format
        re="^[0-9]+.[a-z]+$"
        if ! [[ $name =~ $re ]]; then
            continue
        fi
        # extract id and extension
        id=$(echo "$name" | cut -d '.' -f 1)
        extension=$(echo "$name" | cut -d '.' -f 2)
        # check id range
        tokens=( ${lines[6]} )
        if [ $id -lt ${tokens[0]} ] || [ $id -gt ${tokens[1]} ]; then
            continue
        fi
        # update prev id
        if [ $prev_id -ne $id ]; then
            last_id=$(( id - 1 ))
            for i in $( seq $prev_id $last_id ); do
                echo "$i,0,0,0,missing submission" >> marks.csv
            done
        fi
        prev_id=$(( id + 1 ))
        # check extension
        if ! [[ ${ALLOWED_EXTENSIONS[@]} =~ $extension ]]; then
            tot_mark=$(( -1 * PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            if [[ "$extension" = "zip" || "$extension" = "rar" || "$extension" = "tar" ]]; then
                echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #2" >> marks.csv
                mv  $path ./issues/
            elif [[ "$extension" = "c" || "$extension" = "cpp" || "$extension" = "py" || "$extension" = "sh" ]]; then
                echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #3" >> marks.csv
                mv  $path ./issues/
            elif [[ $USE_ARCHIVE = "true" ]]; then
                echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #2" >> marks.csv
                mv  $path ./issues/
            else
                echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #3" >> marks.csv
                mv  $path ./issues/
            fi
            continue
        fi
        STUDENT_DIRECTORY="${WORKING_DIRECTORY}/${id}"
        mkdir -p $STUDENT_DIRECTORY
        
        # unzip
        if [[ $extension = "zip" ]]; then
            unzip -o -q $path -d $STUDENT_DIRECTORY
        # untar
        elif [[ $extension = "tar" ]]; then
            tar -xvf $path -C $STUDENT_DIRECTORY
        # unrar
        elif [[ $extension = "rar" ]]; then
            unrar e -y $path $STUDENT_DIRECTORY
        # move file
        else
            mv $path $STUDENT_DIRECTORY
        fi

        # check if unzipped files include any valid file
        gotfile=0
        subfiles=()
        while IFS= read -r file; do
            subfiles+=("$file")
        done < <(find $STUDENT_DIRECTORY -type f)
        for subfilename2 in ${subfiles[@]}; do
            subname2=$(basename "$path")
            re="^[0-9]+.[a-z]+$"
            if [[ $subname2 =~ $re ]]; then
                subid2=$(echo "$name" | cut -d '.' -f 1)
                subextension2=$(echo "$name" | cut -d '.' -f 2)
                if [[ ${ALLOWED_EXTENSIONS[@]} =~ $extension ]]; then
                    gotfile=1
                    break
                fi
                if [[ "$extension" = "c" || "$extension" = "cpp" || "$extension" = "py" || "$extension" = "sh" ]]; then
                    gotfile=-1
                fi
            fi
        done
        if [ $gotfile -eq 0 ]; then
            tot_mark=$(( -1 * PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            echo "$id, 0, $PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION, $tot_mark, missing submission in zipped file;" >> marks.csv
            mv  $STUDENT_DIRECTORY ./issues/
            continue
        elif [ $gotfile -eq -1 ]; then
            tot_mark=$(( -1 * PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #3" >> marks.csv
            mv  $STUDENT_DIRECTORY ./issues/
            continue
        fi

    
    # directory
    else
        re="^[0-9]+$"
        if ! [[ $name =~ $re ]]; then
            continue
        fi
        # check id range
        tokens=( ${lines[6]} )
        if [ $name -lt ${tokens[0]} ] || [ $name -gt ${tokens[1]} ]; then
            continue
        fi

        id=$name
        if [[ $USE_ARCHIVE = "true" ]]; then
            case_1=1
        fi

        # update prev id
        if [ $prev_id -ne $id ]; then
            last_id=$(( id - 1 ))
            for i in $( seq $prev_id $last_id ); do
                echo "$i,0,0,0,missing submission" >> marks.csv
            done
        fi
        prev_id=$(( id + 1 ))

    fi

    # after student directory is created
    STUDENT_DIRECTORY="${WORKING_DIRECTORY}/${id}"

    # get all files within student directory
    subfiles=()
    while IFS= read -r file; do
        subfiles+=("$file")
    done < <(find $STUDENT_DIRECTORY -type f)

    # iterate all files in student directory
    for subpath in ${subfiles[@]}; do
        subname=$(basename "$subpath")
        # check filename format
        re="^[0-9]+.[a-z]+$"
        if ! [[ $subname =~ $re ]]; then
            continue
        fi
        # extract id and extension
        subid=$(echo "$subname" | cut -d '.' -f 1)
        subextension=$(echo "$subname" | cut -d '.' -f 2)
        # check id
        if [ $subid -ne $id ]; then
            continue
        fi
        # check extension
        if ! [[ "$subextension" = "c" || "$subextension" = "cpp" || "$subextension" = "py" || "$subextension" = "sh" ]]; then
            continue
        fi
        if ! [[ ${ALLOWED_PROGRAMMING_LANGUAGES[@]} =~ $subextension ]]; then
            tot_mark=$(( -1 * PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            echo "$id,0,$PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION,$tot_mark,issue case #3" >> marks.csv
            mv  $path ./issues/
            break
        fi
        TEMP_FILE="${STUDENT_DIRECTORY}/${subname}.out"
        OUTPUT_FILE="${STUDENT_DIRECTORY}/${subid}_output.txt"
        # run file
        if [[ $subextension = "c" ]]; then
            gcc $subpath -o $TEMP_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,compilation error;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
            $TEMP_FILE > $OUTPUT_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,runtime error;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
        elif [[ $subextension = "cpp" ]]; then
            g++ $subpath -o $TEMP_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,compilation error;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
            $TEMP_FILE > $OUTPUT_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,runtime error;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
        elif [[ $subextension = "py" ]]; then
            python3 $subpath > $OUTPUT_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,failed to interprete;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
        elif [[ $subextension = "sh" ]]; then
            bash $subpath > $OUTPUT_FILE
            if [[ $? -ne 0 ]]; then
                echo "$id,0,0,0,failed to interprete;" >> marks.csv
                mv  $STUDENT_DIRECTORY ./issues/
                break
            fi
        fi
        


        # marking and finalizing
        marks_deducted=0
        mr_ded=0
        remark=""
        # case 1
        if [ $case_1 -eq 1 ]; then
            mr_ded=$(( marks_deducted + PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            remark="issue case #1"
        # case 4
        elif [[ $USE_ARCHIVE = "true" ]] && [ ! -d "${STUDENT_DIRECTORY}/${id}" ]; then
            mr_ded=$(( marks_deducted + PENALTY_FOR_SUBMISSION_GUIDELINES_VIOLATION ))
            remark="issue case #4"
        fi

        # make array of output line
        mapfile out_lines < $OUTPUT_FILE
        for i in "${!out_lines[@]}"; do
            line=${out_lines[$i]}
            line=$(echo "$line" | tr -d '\n' | tr -d '\r' | tr -d '\t')
            line="${line%% }"
            line="${line# }"
            out_lines[$i]=$line
        done

        # count deduction for missed lines
        for i in "${!expected_lines[@]}"; do
            if [ $i -ge ${#out_lines[@]} ]; then
                marks_deducted=$(( marks_deducted + PENALTY_FOR_UNMATCHED_OUTPUT ))
                continue
            fi
            line1=${out_lines[$i]}
            out_lines[$i]=$(echo "$line1" | tr -d '\n' | tr -d '\r')
            if [[ ${expected_lines[$i]} != ${out_lines[$i]} ]]; then
                marks_deducted=$(( marks_deducted + PENALTY_FOR_UNMATCHED_OUTPUT ))
            fi
        done
        # intended to make the marks non negative
        # if [ $marks_deducted -gt $TOTAL_MARKS ]; then
        #     marks_deducted=$TOTAL_MARKS
        # fi

        marks=$(( TOTAL_MARKS - marks_deducted))

        # plagiarized or not
        tot_mark=$marks
        if [[ ${plag_ids[@]} =~ $id ]]; then
            tot_mark=$(( -1 * TOTAL_MARKS * PLAGIARISM_PENALTY / 100))
            # marks_deducted=$(( marks_deducted + pleg_deduct ))
            remark="${remark}plagiarism detected"
        fi
        tot_mark=$(( tot_mark - mr_ded ))
        
        echo "$subid,$marks,$mr_ded,$tot_mark,$remark" >> marks.csv
    done
    mv $STUDENT_DIRECTORY ./checked/
done



last_id=( $STUDENT_ID_RANGE )
last_id=${last_id[1]}

if [ $prev_id -ne $last_id ]; then
    for i in $( seq $prev_id $last_id ); do
        echo "$i,0,0,0,missing submission" >> marks.csv
    done
fi