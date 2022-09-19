# Savodliy

A simple server which checks words with Uzbek dictionary, gives suggestions or adds words to the dictionary.

The API is publicly available at this domain: `savodliy.spider.uz`

## Check word

`GET /check?word=<word>`

**Query Params:**

`word` - word for checking

**Responses:**

`Content-Type: application/json`

**200:** `"correct"`

**400:** `"Invalid word"`

**404:** `"not correct"`

## Get suggestions

`GET /suggest?word=<word>&limit=<limit>`

**Query Params:**

`word` - word for checking

`limit` - optional parameter, number of words to suggest, range is [1, 30]

**Responses:**

`Content-Type: application/json`

**200:** `["word1", "word2", ...]`

**400:** `"Invalid word"`

**400:** `"Limit should be in range [1, 30]"`

## Make suggestions

`POST /suggest?word=<word>`

**Query Params:**

`word` - word for checking

**Responses:**

`Content-Type: application/json`

**200:** `"success"`

**400:** `"Invalid word"`

**500:** `"Server error"`
