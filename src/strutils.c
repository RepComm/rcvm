
#ifndef STRUTILS_C
#define STRUTILS_C 1

#ifndef str
#define str char *
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #include <stdio.h>

struct str_split_info {
  /* The string to be split
   * Provided by caller of str_split_begin function
   */
  str source;
  /* The string that cuts the source string, all occurances of
   * this string will be removed from the source string

   * Provided by caller of str_split_begin function
   */
  str delimiter;

  /* Array of strings split by delimiter
   * Provided and allocated by str_split_begin function
   * Must be garbage collected by str_split_end function
   */
  str * splitStrings;

  /* Array of string lengths split by delimiter
    * Provided and allocated by str_split_begin function
    * Must be garbage collected by str_split_end function
    */
  int * splitStringsLengths;

  /* Number of strings split by delimiter contained in splitStrings
   * Provided by str_split_begin function
   */
  int splitStringsCount;
};
#define str_split_infop struct str_split_info *

/* Split a string by a delimiting string
 * 
 * The caller is responsible only for calling str_split_end
 * when finished with the results in 'info'
 */
void str_split_begin (str_split_infop info) {
  info->splitStringsCount = 0;

  int sourceLength = strlen(info->source);
  if (sourceLength < 1) return;
  int sourceOffset = 0;
  char sourceChar;

  int delimiterLength = strlen(info->delimiter);
  int delimiterOffset = 0;
  char delimiterChar;

  //first pass, simply count occurances so we can allocate only once
  for (sourceOffset = 0; sourceOffset<sourceLength; sourceOffset++) {
    sourceChar = info->source[sourceOffset];
    delimiterChar = info->delimiter[delimiterOffset];

    if (sourceChar == delimiterChar) {
      delimiterOffset++;

      if (delimiterOffset >= delimiterLength) {
        delimiterOffset = 0;
        //increment count
        info->splitStringsCount ++;
      }
    } else {
      delimiterOffset = 0;
    }
  }
  info->splitStringsCount++;

  //allocate arrays since we know the count
  //this one is an array of strings, which are each char arrays
  info->splitStrings = (str *) malloc(sizeof (str *) * info->splitStringsCount);
  //this one is an array of ints
  info->splitStringsLengths = (int*) malloc(sizeof(int) *info->splitStringsCount);

  int stringBegin = 0;
  int stringEnd = 0;
  int splitIndex = 0;
  int splitLength = 0;

  //second pass, fill the arrays
  for (sourceOffset = 0; sourceOffset<sourceLength; sourceOffset++) {
    sourceChar = info->source[sourceOffset];
    delimiterChar = info->delimiter[delimiterOffset];

    if (sourceChar == delimiterChar) {
      delimiterOffset++;

      //if we've reached the end of the delimiter
      if (delimiterOffset >= delimiterLength) {

        //don't worry about delimiter trailing null, strlen doesn't count those
        stringEnd = sourceOffset - delimiterLength;
        
        //char count of substring we want to split
        splitLength = stringEnd - stringBegin + 1;

        //allocate for our substring split
        info->splitStrings[splitIndex] = (str) malloc(
          //+1 for trailing null for c-string
          sizeof(char) * splitLength + 1
        );

        //copy substring from source into splitStrings array
        memcpy(
          info->splitStrings[splitIndex],
          info->source + stringBegin,
          splitLength
        );
        //explicitly set the last char of this split to a NULL just for fun
        info->splitStrings[splitIndex][splitLength] = 0x00;

        //conveniently put the substring split size for the
        //user of str_split_begin :)
        info->splitStringsLengths[splitIndex] = splitLength;

        //move to next split index
        splitIndex ++;

        //reset delimiter offset so we look for new occurances of it
        delimiterOffset = 0;

        //next substring split should occur after the current delimiter
        stringBegin = sourceOffset+1;
      }
    } else {
      //reset delimiter offset so we look for new occurances of it
      delimiterOffset = 0;
    }
  }

  //handle edge case of last substring after last delimiter
  if (stringEnd != stringBegin) {
    stringEnd = sourceLength-1;

    splitLength = stringEnd - stringBegin + 1;

    //allocate for our substring split
    info->splitStrings[splitIndex] = (str) malloc(
      //+1 for trailing null for c-string
      sizeof(char) * splitLength + 1
    );

    //copy substring from source into splitStrings array
    memcpy(
      info->splitStrings[splitIndex],
      info->source + stringBegin,
      splitLength
    );
    
  }
}
int str_split_count (str_split_infop info) {
  return info->splitStringsCount;
}

void str_split_get (str_split_infop info, str * out) {
  for (int i=0; i < info->splitStringsCount; i++) {
    strcpy(out[i], info->splitStrings[i]);
  }
}

void str_split_end (str_split_infop info) {
  if (info->splitStringsCount > 0 && info->splitStrings != NULL) {
    //free each string allocated
    for (int i=0; i < info->splitStringsCount; i++) {
      free(info->splitStrings[i]);
    }
    //free string array pointer
    free (info->splitStrings);

    //free string lengths array pointer
    free(info->splitStringsLengths);

    info->splitStringsCount = 0;
  }
}

// void str_split_test () {
  
//   struct str_split_info info;
//   info.source = "hello world this is a test";
//   info.delimiter = " ";

//   str_split_begin (&info);

//   //iterate thru split substrings
//   //NOTE: removed/memory cleanup after str_split_end
//   for (int i=0; i<info.splitStringsCount; i++) {
//     //  info.splitStrings[i];
//   }

//   str_split_end(&info);
// }

bool str_equal (const char *a, const char *b) { return strcmp(a, b) == 0; }

#endif