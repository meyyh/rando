#!/bin/bash

if [ $# -eq 0 ]; then
    echo "run with \"i\" or \"input\" to input a uniart into the db
or
run with \"o\" or \"output\" to search and output a uniart"
    exit 1
fi

if ! command -v gum &> /dev/null; then
    echo "gum is not installed. Exiting."
    exit 1
fi

if [ "$XDG_SESSION_TYPE" == "wayland" ]; then
    if ! command -v wl-paste &> /dev/null; then
        echo "Install wl-clipboard"
        exit 1
    fi
elif [ "$XDG_SESSION_TYPE" == "x11" ]; then
    if ! command -v xclip &> /dev/null; then
        echo "Install xclip"
        exit 1
    fi
else
    if command -v xclip &> /dev/null; then
        XDG_SESSION_TYPE="x11"
    elif command -v wl-paste &> /dev/null; then
        XDG_SESSION_TYPE="wayland"
    else
        echo "Unknown display protocol and clipboard"
        exit 1
    fi
fi

db_file=uniart.json

if [ "$1" == "i" ] || [ "$1" == "input" ]; then
    echo "is the uniart in your clipboard"
    gum confirm && echo "getting clipbaord" || exit 1

    if [ ! -f "$db_file" ]; then
        echo '{}' > $db_file
    fi

    if [ "$XDG_SESSION_TYPE" == "wayland" ]; then
        clipboard_content=$(wl-paste)
    elif [ "$XDG_SESSION_TYPE" == "x11" ]; then
        clipboard_content=$(xclip -o)
    fi

    utf8_encoding=""
    for (( i=0; i<${#clipboard_content}; i++ )); do
        char="${clipboard_content:$i:1}"
        # Convert the character to its UTF-8 encoding and append it
        utf8_encoding+=$(printf '%s' "$char" | od -An -tx1 | tr '[:lower:]' '[:upper:]' | sed 's/ /\\x/g' | tr -d '\n')
    done

    name=$(gum input --placeholder "name for entry")
    tags_input=$(gum input --placeholder "list tags with a space in between")
    # Convert the tags input into an array by splitting on spaces
    tags=($tags_input)

    if jq -e ".\"$name\"" $db_file > /dev/null; then
        echo "The entry with the name '$name' already exists. Use a different name or update the existing entry."
        exit 1
    else
        jq -r --arg name "$name" --arg uniart "$utf8_encoding" --argjson tags "$(printf '%s\n' "${tags[@]}" | jq -R . | jq -s .)" \
        '. + {($name): { "uniart": $uniart, "tags": $tags }}' $db_file > temp.json && mv temp.json $db_file
    fi
fi

if [ "$1" == "o" ] || [ "$1" == "output" ]; then
    echo "search based on"
    search=$(gum choose "tag" "name" "dump")

    name_list=$(jq -r 'keys[]' "$db_file" | sort -u)
    tag_list=$(jq -r '.[] | (.tags[])' "$db_file" | sort -u)
    num_lines=$(( $(tput lines) - 3 ))

    if [ "$search" == "tag" ]; then
        search_value=$(gum choose $tag_list --height=$num_lines)
    elif [ "$search" == "name" ]; then
        search_value=$(gum choose $name_list --height=$num_lines)
    elif [ "$search" == "dump" ]; then
        for entry in $(jq -r 'keys[]' "$db_file"); do
            uniart=$(jq -r --arg entry "$entry" '.[$entry].uniart' "$db_file")
            tags=$(jq -r --arg entry "$entry" '.[$entry].tags | join(", ")' "$db_file")

            echo "Name: $entry"
            echo "Tags: $tags"
            printf "$uniart\n" | sed 's/\\\\/\\/g'
            echo "---------------------------"
        done
    fi
    
    uniart=$(jq -r --arg tag "$search_value" 'to_entries[] | select(.value.tags[]? == $tag) | .value.uniart' "$db_file")
    printf "$uniart\n" | sed 's/\\\\/\\/g'
fi