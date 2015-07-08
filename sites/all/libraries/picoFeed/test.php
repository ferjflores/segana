<?php

require 'vendor/autoload.php';

use PicoFeed\Reader\Reader;
use PicoFeed\Config\Config;
use PicoFeed\Scraper\Scraper;

$config = new Config;

$grabber = new Scraper($config);
$grabber->setUrl("http://noticiaaldia.com/");
$grabber->execute();

// Get raw HTML content
echo $grabber->getRawContent();

// Get relevant content
echo $grabber->getRelevantContent();

// Get filtered relevant content
echo $grabber->getFilteredContent();

// Return true if there is relevant content
var_dump($grabber->hasRelevantContent());




function tidyHTML5($buffer)
{
    $buffer = str_replace('<menu', '<mytag', $buffer);
    $buffer = str_replace('menu>', 'mytag>', $buffer);
    $tidy = new tidy();
    $options = array(
      'hide-comments'         => true,
      //'tidy-mark'             => false,
      //'indent'                => true,
      //'indent-spaces'         => 4,
      'new-blocklevel-tags'   => 'menu,mytag,article,header,footer,section,nav',
      'new-inline-tags'       => 'video,audio,canvas,ruby,rt,rp',
      //'doctype'               => '<!DOCTYPE HTML>',
      //'sort-attributes'     => 'alpha',
      //'vertical-space'        => false,
      //'output-xhtml'          => true,
      //'wrap'                  => 180,
      //'wrap-attributes'       => false,
      //'break-before-br'       => false,
      'char-encoding'         => 'utf8',
      'input-encoding'        => 'utf8',
      'output-encoding'       => 'utf8',
      'show-body-only' => true
    );

    $tidy->parseString($buffer, $options, 'utf8');
    $tidy->cleanRepair();

    //$html = '<!DOCTYPE HTML>' . PHP_EOL . $tidy->html();
    $html = $tidy->html();
    $html = str_replace('<html lang="en" xmlns="http://www.w3.org/1999/xhtml">', '<html>', $html);
    $html = str_replace('<html xmlns="http://www.w3.org/1999/xhtml">', '<html>', $html);

    //Hackish stuff starts here
    //We use regex to parse html, which is usually a bad idea
    //But currently there is no alternative to it, since tidy is not MENU TAG friendly
    preg_match_all('/\<mytag(?:[^\>]*)\>\s*\<ul>/', $html, $matches);
    foreach($matches as $m) {
        $mo = $m;
        $m = str_replace('mytag', 'menu', $m);
        $m = str_replace('<ul>', '', $m);
        $html = str_replace($mo, $m, $html);
    }
    $html = str_replace('<mytag', '<menu', $html);
    $html = str_replace('</ul></mytag>', '</menu>', $html);
    $html = str_replace('mytag>', 'menu>', $html);
    return $html;
}