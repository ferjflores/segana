<?php
function forceUTF8($text){
        /**
         * Function forceUTF8
         *
         * This function leaves UTF8 characters alone, while converting almost all non-UTF8 to UTF8.
         *
         * It may fail to convert characters to unicode if they fall into one of these scenarios:
         *
         * 1) when any of these characters:   ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß
         *    are followed by any of these:  ("group B")
         *                                    ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶•¸¹º»¼½¾¿
         * For example:   %ABREPRESENT%C9%BB. «REPRESENTÉ»
         * The "«" (%AB) character will be converted, but the "É" followed by "»" (%C9%BB)
         * is also a valid unicode character, and will be left unchanged.
         *
         * 2) when any of these: àáâãäåæçèéêëìíîï  are followed by TWO chars from group B,
         * 3) when any of these: ðñòó  are followed by THREE chars from group B.
         *
         * @name forceUTF8
         * @param string $text  Any string.
         * @return string  The same string, UTF8 encoded
         *
         */

        if(is_array($text))
        {
                foreach($text as $k => $v)
                {
                        $text[$k] = forceUTF8($v);
                }
                return $text;
        }

        $max = strlen($text);
        $buf = "";
        for($i = 0; $i < $max; $i++){
                $c1 = $text{$i};
                if($c1>="\xc0"){ //Should be converted to UTF8, if it's not UTF8 already
                        $c2 = $i+1 >= $max? "\x00" : $text{$i+1};
                        $c3 = $i+2 >= $max? "\x00" : $text{$i+2};
                        $c4 = $i+3 >= $max? "\x00" : $text{$i+3};
                        if($c1 >= "\xc0" & $c1 <= "\xdf"){ //looks like 2 bytes UTF8
                                if($c2 >= "\x80" && $c2 <= "\xbf"){ //yeah, almost sure it's UTF8 already
                                        $buf .= $c1 . $c2;
                                        $i++;
                                } else { //not valid UTF8.  Convert it.
                                        $cc1 = (chr(ord($c1) / 64) | "\xc0");
                                        $cc2 = ($c1 & "\x3f") | "\x80";
                                        $buf .= $cc1 . $cc2;
                                }
                        } elseif($c1 >= "\xe0" & $c1 <= "\xef"){ //looks like 3 bytes UTF8
                                if($c2 >= "\x80" && $c2 <= "\xbf" && $c3 >= "\x80" && $c3 <= "\xbf"){ //yeah, almost sure it's UTF8 already
                                        $buf .= $c1 . $c2 . $c3;
                                        $i = $i + 2;
                                } else { //not valid UTF8.  Convert it.
                                        $cc1 = (chr(ord($c1) / 64) | "\xc0");
                                        $cc2 = ($c1 & "\x3f") | "\x80";
                                        $buf .= $cc1 . $cc2;
                                }
                        } elseif($c1 >= "\xf0" & $c1 <= "\xf7"){ //looks like 4 bytes UTF8
                                if($c2 >= "\x80" && $c2 <= "\xbf" && $c3 >= "\x80" && $c3 <= "\xbf" && $c4 >= "\x80" && $c4 <= "\xbf"){ //yeah, almost sure it's UTF8 already
                                        $buf .= $c1 . $c2 . $c3;
                                        $i = $i + 2;
                                } else { //not valid UTF8.  Convert it.
                                        $cc1 = (chr(ord($c1) / 64) | "\xc0");
                                        $cc2 = ($c1 & "\x3f") | "\x80";
                                        $buf .= $cc1 . $cc2;
                                }
                        } else { //doesn't look like UTF8, but should be converted
                                $cc1 = (chr(ord($c1) / 64) | "\xc0");
                                $cc2 = (($c1 & "\x3f") | "\x80");
                                $buf .= $cc1 . $cc2;
                        }
                } elseif(($c1 & "\xc0") == "\x80"){ // needs conversion
                        $cc1 = (chr(ord($c1) / 64) | "\xc0");
                        $cc2 = (($c1 & "\x3f") | "\x80");
                        $buf .= $cc1 . $cc2;
                } else { // it doesn't need convesion
                        $buf .= $c1;
                }
        }
        return $buf;
}

function forceLatin1($text) {
        if(is_array($text)) {
                foreach($text as $k => $v) {
                        $text[$k] = forceLatin1($v);
                }
                return $text;
        }
        return utf8_decode(forceUTF8($text));
}

function fixUTF8($text){
        if(is_array($text)) {
                foreach($text as $k => $v) {
                        $text[$k] = fixUTF8($v);
                }
                return $text;
        }

        $last = "";
        while($last <> $text){
                $last = $text;
                $text = forceUTF8(utf8_decode(forceUTF8($text)));
        }
        return $text;
}



function _get_query_string(SelectQueryInterface $query) {
  $string = (string) $query;
  $arguments = $query->arguments();

  if (!empty($arguments) && is_array($arguments)) {
    foreach ($arguments as $placeholder => &$value) {
      if (is_string($value)) {
        $value = "'$value'";
      }
    }

    $string = strtr($string, $arguments);
  }

  return $string;
}
function ctype_alnum_portable($text) {
    return (preg_match("/[[:alnum:]]/", $text) > 0);
}

function sin_acentos($text){
        $unwanted_array = array(    'Š'=>'S', 'š'=>'s', 'Ž'=>'Z', 'ž'=>'z', 'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
                            'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O', 'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U',
                            'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss', 'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c',
                            'è'=>'e', 'é'=>'e', 'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o', 'ô'=>'o', 'õ'=>'o',
                            'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'ý'=>'y', 'þ'=>'b', 'ÿ'=>'y' );
        $text = strtr( $text, $unwanted_array );
        return $text;
}

function convert_smart_quotes($string) 
{ 
    $search = array(chr(145), 
                    chr(146), 
                    chr(147), 
                    chr(148), 
                    chr(151)); 

    $replace = array("'", 
                     "'", 
                     '"', 
                     '"', 
                     '-');
  /*$search = array(
                  '&',
                  '<',
                  '>',
                  '"',
                  chr(212),
                  chr(213),
                  chr(210),
                  chr(211),
                  chr(209),
                  chr(208),
                  chr(201),
                  chr(145),
                  chr(146),
                  chr(147),
                  chr(148),
                  chr(151),
                  chr(150),
                  chr(133)
                 );

  $replace = array(
                  '&amp;',
                  '&lt;',
                  '&gt;',
                  '&quot;',
                  '&#8216;',
                  '&#8217;',
                  '&#8220;',
                  '&#8221;',
                  '&#8211;',
                  '&#8212;',
                  '&#8230;',
                  '&#8216;',
                  '&#8217;',
                  '&#8220;',
                  '&#8221;',
                  '&#8211;',
                  '&#8212;',
                  '&#8230;'
                 );*/
    return str_replace($search, $replace, $string); 
}

/**
 *  â€˜  8216  curly left single quote
 *  â€™  8217  apostrophe, curly right single quote
 *  â€œ  8220  curly left double quote
 *  â€  8221  curly right double quote
 *  â€”  8212  em dash
 *  â€“  8211  en dash
 *  â€¦  8230  ellipsis
 */
 /*
$search = array(
                '&',
                '<',
                '>',
                '"',
                chr(212),
                chr(213),
                chr(210),
                chr(211),
                chr(209),
                chr(208),
                chr(201),
                chr(145),
                chr(146),
                chr(147),
                chr(148),
                chr(151),
                chr(150),
                chr(133)
               );

$replace = array(
                '&amp;',
                '&lt;',
                '&gt;',
                '&quot;',
                '&#8216;',
                '&#8217;',
                '&#8220;',
                '&#8221;',
                '&#8211;',
                '&#8212;',
                '&#8230;',
                '&#8216;',
                '&#8217;',
                '&#8220;',
                '&#8221;',
                '&#8211;',
                '&#8212;',
                '&#8230;'
               );
*/




/**
 * Logging class:
 * - contains lfile, lwrite and lclose public methods
 * - lfile sets path and name of log file
 * - lwrite writes message to the log file (and implicitly opens log file)
 * - lclose closes log file
 * - first call of lwrite method will open log file implicitly
 * - message is written with the following format: [d/M/Y:H:i:s] (script name) message
 */

?>

