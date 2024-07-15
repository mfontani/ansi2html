#!/bin/bash
n_ok=0
n_bad=0
n_test=0
str_eq_html () {
    n_test=$((n_test + 1))
    printf '# %-2d STR:  %s\n' "$n_test" "$(printf '%s' "$1" | cat -v)"
    printf '# %-2d WANT: %s\n' "$n_test" "$(printf '%s' "$2" | cat -v)"
    printf '# %-2d GOT:  %s\n' "$n_test" "$(printf '%s' "$3" | cat -v)"
    if [[ "$2" == "$3" ]]; then
        printf 'ok %d\n' "$n_test"
        n_ok=$((n_ok + 1))
    else
        printf 'not ok %d\n' "$n_test"
        n_bad=$((n_bad + 1))
    fi
}
done_testing() {
    printf '1..%d\n' "$n_test"
    printf '# File: %s - pass %d/%d, fail %d\n' "$0" "$n_ok" "$n_test" "$n_bad"
}
