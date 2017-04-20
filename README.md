# libeav - Email Address Validator Library


## Description

This is a library which validates an [Email Address][1]. I do my best to
support all existing RFCs:

* [RFC 822][2]
* [RFC 5321][3]
* [RFC 5322][4] (updates `<atext>` specification)
* [RFC 6321][5] (UTF-8 support; updates `<atext>` specification)


A quick note about [RFC 6321][5]: it is possible that the correct implementation
SHOULD take into account [RFC 20][6] to exclude some characters:
`#`, `\``, `{`, `}`, `|`, `~` and `^`. I did it, but you may change this
in the way you would like (see the [is_6321_local.c][7] file).


## What it does not

You have to be NOT mistaken about the definition of [Email Address][1].
This software does NOT implement (at the moment, at least) a full support
of [Address Specification][14].

All that this software does it is validating of the [Addr-Spec Specification][15].
See details below:

	address = mailbox / group
	mailbox = name-addr / addr-spec
	addr-spec = local-part "@" domain


## Dependencies

* [idnkit][8]
* [GNU make][10] to build
* C99 compiler to compile


## Build

First of all, you have to change path to [idnkit][8] in the `Makefile`, in
case if did not this already.

```
shell> make
shell> make check
shell> make install DESTDIR=/path/to/install
```

The default destination of `DESTDIR` is `/usr/local`.


## Tools

There is an utility called `eav`. It will be installed by default. It has
a simple usage:

```
shell> eav emails1.txt emails2.txt
```

See its sources in the [bin][12] directory.

The sample output is:

```
FAIL: localpart-only
      email has no domain part
PASS: i.have.only.ipv4@[8.8.8.8]
PASS: this-is.ipv6@[IPv6:2001:0db8:0000:0000:0000:ff00:0042:8329]
PASS: and.this.is.ipv6-ipv4@[IPv6:::ffff:192.0.2.128]
FAIL: invalid.ipv4@[0.1.2.3]
      invalid ip address
FAIL: invalid.ipv6@[IPv6:ge80:1::]
      invalid ip address
PASS: valid.ipv6@[IPv6:2001:db8:0:1:1:1:1:1]
PASS: legacy.valid.ipv6@[IPv6:2001:db8::1:1:1:1:1]
PASS: xxx.ipv6@[2001:db8:1:1:1:1:1:1]
PASS: simple@test.com
```


## Credits

* [UTF-8 decoder][11] by JSON.org
* [postfix][13] written by Wietse Venema


## Feature plans

* Bug fixing
* Add support to [libidn][9]


## License

This software is licensed under "The 2-Clause BSD License".


[1]: https://en.wikipedia.org/wiki/Email_address
[2]: https://tools.ietf.org/html/rfc822
[3]: https://tools.ietf.org/html/rfc5321
[4]: https://tools.ietf.org/html/rfc5322
[5]: https://tools.ietf.org/html/rfc6321
[6]: https://tools.ietf.org/html/rfc20
[7]: /src/is_6321_local.c
[8]: https://jprs.co.jp/idn/index-e.html
[9]: https://www.gnu.org/software/libidn/
[10]: https://www.gnu.org/software/make/
[11]: http://www.json.org/JSON_checker/
[12]: /bin/
[13]: http://www.postfix.org/
[14]: https://tools.ietf.org/html/rfc5322#section-3.4
[15]: https://tools.ietf.org/html/rfc5322#section-3.4.1
