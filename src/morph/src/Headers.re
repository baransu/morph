type t = list((string, string));

let empty = [];

let get_header = (key, headers) =>
  headers
  |> List.find_opt(((header, _)) =>
       String.lowercase_ascii(header) == String.lowercase_ascii(key)
     )
  |> Utils.map_opt(snd);

let add_header = (new_header, headers) => 
  headers @ [new_header];

let add_headers = (new_headers, headers: t) =>
  headers @ new_headers;
