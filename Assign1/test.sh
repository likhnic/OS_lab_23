(cat $1; echo)| while IFS= read -r line; do
    if(grep -q '^[^"]*\("[^"]*"\)*[^"]*#' <<<"$line" && grep -q "^[^']*\('[^']*'\)*[^']*#" <<<"$line");then
        # printf "%s\n" "$line"
        echo `grep -ne '^[^"]*\("[^"]*"\)*[^"]*#' <<<"$line"`
        # echo `grep -q '[^"]*\("[^"]*"\)*[^"]*#' <<<"$line"`
    fi
done 
