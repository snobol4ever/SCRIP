class Pod::Block { has %.config; has @.contents }
class Pod::Block::Named is Pod::Block { has %.config; has @.contents; has $.name }
class Pod::Block::Para is Pod::Block { has %.config; has @.contents }
class Pod::Heading is Pod::Block { has %.config; has @.contents; has $.level }
class Pod::Block::Code is Pod::Block { has %.config; has @.contents }
class Pod::Block::Comment is Pod::Block { has %.config; has @.contents }
class Pod::Item is Pod::Block { has %.config; has @.contents; has $.level }
class Pod::Block::Declarator is Pod::Block { has %.config; has @.contents; has $.WHEREFORE; has @.leading; has @.trailing; method Str { @.leading.join("\n") }; method gist { self.Str } }
