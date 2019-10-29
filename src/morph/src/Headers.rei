type t = list((string, string));

let empty: t;

let get_header: (string, t) => option(string);

let add_header: ((string, string), t) => t;

let add_headers: (list((string, string)), t) => t;
