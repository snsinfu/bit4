#!/bin/sh -efu

LANG=C
LC_ALL=C

main() {
    qstat -f1 | qstat_to_json
}

qstat_to_json() {
    awk '
    BEGIN {
        OFS = "\t"
        parse_state = "init"
    }

    # Start of new job table
    parse_state == "init" && /^Job Id:/ {
        job_id = substr($0, 9)
        for (key in job_attrs) {
            delete job_attrs[key]
        }
        parse_state = "entry"
    }

    # New attribute entry
    parse_state == "entry" && /^    / {
        record = substr($0, 5)
        split_position = index(record, "=")
        key = substr(record, 1, split_position - 2)
        value = substr(record, split_position + 2)
        job_attrs[key] = value
    }

    # End of job table
    parse_state == "entry" && length == 0 {
        attrs = "\"id\": \"" job_id "\""
        for (key in job_attrs) {
            attrs = attrs ", \"" key "\": \"" job_attrs[key] "\""
        }
        print "{" attrs "}"
        parse_state = "init"
    }'
}

main "$@"
