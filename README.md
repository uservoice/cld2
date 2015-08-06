# Compact Language Detection 2.0

[![Gem Version](https://badge.fury.io/rb/cld2.svg)](http://badge.fury.io/rb/cld2)

![image](https://circleci.com/gh/BanjoInc/cld2.png?circle-token=6e9c5831521447a5005be3f4d33a221e9d2ae1d4)

Based on Jason Toy's CLD v1.0.
Blazing-fast language detection for Ruby provided by Google Chrome's Compact Language Detector v2.0

## How to Use

```ruby
CLD.detect_language("plus ça change, plus c'est la même chose")
# => {:name => "FRENCH", :code => "fr", :reliable => true}

Verbose results (optional, defaulted to false): also return up to 3 top languages detected for the document and their respective scores, as well as individual results for each chunk from the input text.
CLD.detect_language("How much wood would a woodchuck chuck", true)
# => {:name=>"ENGLISH", :code=>"en", :reliable=>true, :top_langs=>[{:code=>"en", :percent=>97, :score=>943.0}], :chunks=>[{:content=>"How much wood would a woodchuck chuck", :code=>"un"}]} 

CLD.detect_language("हैदराबाद उच्चार ऐका सहाय्य माहिती तेलुगू హైదరాబాదు حیدر آباد", true)
# => {:name=>"MARATHI", :code=>"mr", :reliable=>true, :top_langs=>[{:code=>"mr", :percent=>69, :score=>387.0}, {:code=>"te", :percent=>18, :score=>1024.0}], :chunks=>[{:content=>"हैदराबाद उच्चार ऐका सहाय्य माहिती तेलुगू ", :code=>"mr"}, {:content=>"హైదరాబాదు ", :code=>"te"}, {:content=>"حیدر آباد", :code=>"un"}]}


```

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'cld2', require 'cld'
```

And then execute:

```sh
$ bundle
```

## Thanks

Thanks to the Chrome authors, and to Mike McCandless for writing a Python version.
Thanks to Jason Toy for the original cld v1.0 ruby port.

Licensed the same as Chrome.
