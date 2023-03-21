# Numbers in Alphabetical Order

## Caution  

The output file this program generates is **huge** - about 95 GB.
Make sure you have plenty of room (and a fast drive write speed helps).  
The program outputs to the console the number of lines written every million lines for you to track the progress.

[Here is a Google Drive link to the compressed output](https://drive.google.com/file/d/18ENuaTcvvFyMcroOFhm00FtjtSEOEb4x/view?usp=share_link)  
Compressed size: 1.7 GB  
Uncompressed size: 95 GB

------

## Background

Of all positive whole numbers, which one comes first alphabetically?  
It seems like an impossible question to answer, there's an infinite number of positive whole numbers after all.
Maybe an easier question is "which one comes last"? The answer is "Zero" of course, because no other number starts with a 'z'.  
That said, there are some rules to how a number must be named:  

1. Do not use 'and'. It's not "one hundred and twelve", it's "one hundred twelve".
2. 'One hundred', not 'a hundred'. Same for thousand, million, etc.
3. Names of large numbers use the "standard" set by Conway and Guy. See [this wikipedia section](https://en.wikipedia.org/wiki/Names_of_large_numbers#Extensions_of_the_standard_dictionary_numbers).
4. "Alphabetical order" here refers to lexicographic order used by computers to compare ascii strings. That is, a shorter word comes before a ' ' (space) character, which comes before 'a'. Not case sensitive however.  

From that we can see that the first number of all positive integers alphabetically is "eight".
There are no numbers that start with a-d, and the alphabetically first number starting with 'e' is "eight".
There are lots of other numbers that start with "eight", but as outlined previously, the shortest comes first.
After eight, there is no standard "-illion" prefix that starts with 'a', but there's one that starts with 'b': "billion".
Therefore, the second positive integer alphabetically is "eight billion".
After that would follow every number starting with "eight billion" - theres one billion of them to be precise.

The last one of those is "eight billion two thousand two hundred two".  
What comes next?  
Beyond centillion (10^3(100)+3^), prefixes begin to repeat.
The alphabetically first of such prefixes is "centilli-". One centillicentillion is 10^3(100,100)+3^, One centillicentillicentillion is 10^3(100,100,100)+3^.
But you'll notice that the longer one is first alphabetically, because the 'c' in the next "centilli" comes before the 'o' in "on".
That means that it becomes preferable to keep chaining centilli's than anything else. Therefore, the next number after the last "eight billion two thousand two hundred two" will be "eight centillicentillicentillicentilli..." and so on forever.
Since the number is infinitely large, there's no way to determine the immediate next number.

------

## How it works

Given a certain word that appears in a number, there are only a handful of words that can possibly follow it.
For example, 'Fifty' can only be followed by 'One' through 'Nine' and a magnitude word like 'Million' or 'Thousand'.
So if we start with 'Eight Billion', theres  
**Words that can follow it**:  
A single digit like 'Eight'  
A -ty word like 'Eighty'  
A teen word like 'Eighteen' (note, this also includes 'Ten' because you can't say 'Ten One')  
**Words that *can't* follow**:  
A magnitude like billion, million, or thousand  
Hundred  

Each word has a 'type' that dictates what types of words can follow it. 
The words that can follow each type are stored in a list alphabetically. 
That way, after 'Eight Billion', we can begin iterating through each possible next word alphabetically.
Alphabetically, the next is 'Eight Billion Eight', so we write that, and repeat the process starting with words that can follow 'Eight'.  
Once its written all of the possibilities that can follow 'Eight Billion Eight' (a sortof recursive process), it finally moves on to the next word: 'Eight Billion Eighteen' and repeats again.  

I think this is heavily related to and probably easier to explain with graph theory, but I don't know graph theory and its terminology. 

### How do I know it actually worked right?

I tested the first million lines to make sure they were in alphabetical order, which gave me the confidence to say that everything else is probably in alphabetical order too.
Another big tell was that at first, I had the code wrong, and it would generate too few numbers or too many numbers.  
For example, I forgot 'Forty' and a few other numbers in some of the can-follow lists. This lead to it only generating a couple hundred million lines.
I also had an issue when I forgot to say 'you can't use million if you've used thousand', and it was generating 1.5 billion lines.
I knew I had it right once it generated exactly 1,000,000,001 lines.
I guess its *technically* possible that there are errors that are perfectly cancelling out, or something isnt in the right order wayyy down the list, 
but I would be willing to bet that its output is alphabetically correct and contains every number between 8,000,000,000 and 8,999,999,999 with no duplicates. 
