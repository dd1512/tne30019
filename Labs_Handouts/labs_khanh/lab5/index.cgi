#!/bin/csh

echo "Content-type: text/html"
echo ""

echo "<html><head><title>Aristocrats Family Theater Resturant</title></head><body>"
echo "<center><h1>Hello!</h1></center><p>"
echo "<center><h2>The Aristocrats Family Theater Restaurant site is under construction...</h2></center></p>"
echo "<center><h3>Check back soon for updates.</h3></center></p>"

#do the math
setenv FUTURE `/bin/date -j "200610121300" "+%s"`
setenv TODAY `/bin/date "+%s"`
@ DIFFERENCE = $FUTURE - $TODAY

echo "<center><h3>Remember: It is only <i>$DIFFERENCE</i> seconds until our first location opens!</h3/><center>"
echo "<center><h3>Right now it is "`/bin/date`" and we open on "`/bin/date -r $FUTURE`

echo "</body></html>"
