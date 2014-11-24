<?xml version="1.0"?>
<!--
  Do not use named character entities such as &nbsp; use the numeric form instead e.g &#160;
  The XSLT processor will report syntax errors if you use named character entities.
  -->
<xsl:stylesheet
    version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
  <xsl:param name="ddebug" />
  <xsl:preserve-space elements="*"/>
  <xsl:output method="html" indent="yes" encoding="utf-8" omit-xml-declaration="yes"/>

  <xsl:template match="/">
  <html>
    <xsl:apply-templates select="//root" />
    <body>
    <xsl:apply-templates select="//word" />
    </body>
    <xsl:if test="$ddebug = 'entryfree'">
      <xsl:apply-templates select="//entryFree" />
    </xsl:if>
  </html>
  </xsl:template>

  <!--
   this is not used anymore
   -->
  <xsl:template match="root">
    <p class="rootword"><span class="arabichead rootword"><xsl:value-of  select="@text"/></span>
    <xsl:if test="@quasi = '1'">
      <span class="quasi">Quasi</span>
    </xsl:if>
    </p>
  </xsl:template>

  <xsl:template match="word">
    <xsl:variable name="entrystyle">
    <xsl:choose>
      <xsl:when test="boolean(./@supp = 1)">
        <xsl:text>supplement</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>main</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="@type = 'root'">
    <p>
      <xsl:attribute name="class">rootword <xsl:value-of select="$entrystyle" /></xsl:attribute>
      <span>
      <xsl:attribute name="class">arabichead rootword <xsl:value-of select="$entrystyle" /></xsl:attribute>
      <xsl:value-of  select="@ar"/>
      </span>
      <xsl:if test="@quasi = '1'">
        <span class="quasi">&#160;&#160;(Quasi root)</span>
      </xsl:if>
      <xsl:if test="@quasi = '2'">
        <span class="quasi">&#160;&#160;(also quasi root)</span>
      </xsl:if>
      </p>
      </xsl:when>
      <xsl:otherwise>
    <span>
      <xsl:attribute name="class">wordheading <xsl:value-of select="$entrystyle" /></xsl:attribute>
      <xsl:if test="not(@itype = '') and not(@itype='alphabetical letter')">
        <span class="infl"><xsl:value-of select="@itype" /></span><xsl:text> </xsl:text>
      </xsl:if>
      <xsl:if test="not(@itype = 'alphabetical letter')">
    <span class="arabic"><xsl:value-of select="@ar"/></span>
      </xsl:if>
    </span>
    <xsl:apply-templates select="entryFree" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
<!---
  These use characters from Arabic Presentation Forms-B to show short vowels u/i/a
  Replace &#xhhhhn; by the e.g u will work.
-->

  <xsl:template match="form">
    <xsl:if test="@n='infl'">
      <xsl:choose>
      <xsl:when test="orth/@orig = 'Bu'">
      <span class="infl">&#xfe79;&#160;</span>
      </xsl:when>
      <xsl:when test="orth/@orig = 'Bi'">
      <span class="infl">&#xfe7b;&#160;</span>
      </xsl:when>
      <xsl:when test="orth/@orig = 'Ba'">
      <span class="infl">&#xfe77;&#160;</span>
      </xsl:when>
      <xsl:when test="orth/@orig = 'BN'">
      <span class="infl">&#xfe72;&#160;</span>
      </xsl:when>
      <xsl:when test="orth/@orig = 'BF'">
      <span class="infl">&#xfe71;&#160;</span>
      </xsl:when>
      <xsl:otherwise>
      </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>
  <!---
      From http://stackoverflow.com/questions/14118670/check-type-of-node-in-xsl-template
      This is used in debugging.
  -->
  <xsl:template match="entryFree" priority="99">
    <span class="entry">
    <xsl:for-each select="@*|node()">
      <p><xsl:attribute name="style">
        <xsl:value-of select="position()" />
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="count(.|/)=1">
          <xsl:text>Root</xsl:text>
        </xsl:when>
        <xsl:when test="self::*">
          <xsl:text>Element </xsl:text>
          <xsl:value-of select="name()"/>
        </xsl:when>
        <xsl:when test="self::text()">
          <xsl:text>Text</xsl:text>
        </xsl:when>
        <xsl:when test="self::comment()">
          <xsl:text>Comment</xsl:text>
        </xsl:when>
        <xsl:when test="self::processing-instruction()">
          <xsl:text>PI</xsl:text>
        </xsl:when>
        <xsl:when test="count(.|../@*)=count(../@*)">
          <xsl:text>Attribute </xsl:text> <xsl:value-of select="name()" />
        </xsl:when>
      </xsl:choose>
      </p>
    </xsl:for-each>
    </span>
  </xsl:template>
  <!--
  For the sense separators.
  Lane uses a long underscore for a "to denote a break in the relations of significations"
  and a double long underscore to denote "an extraordinary, or a complete, dissociation."

  Type 'a/A' is a complete break, type 'b/B' otherwise

  -->
  <xsl:template match="sense">
    <xsl:if test="(@type='B') or (@type='b')">
      <span class="bseparator"><xsl:text>&#x2016;</xsl:text> </span>
    </xsl:if>
    <xsl:if test="(@type='A') or (@type='a')">
      <span class="aseparator"><xsl:text>&#x2017;&#x2017;&#x2017;</xsl:text> </span>
    </xsl:if>
  </xsl:template>

  <!--
  For page breaks
  -->
   <xsl:template match="pb">
    <br/>
    <p class="pagebreak">Page <xsl:value-of select="@n" />
    </p>
  <br/>
  </xsl:template>

  <!--
      For single or multiline quotes.
      In the original they appear on separate lines between *'s but occasionally
      he has

      * <arabic text > * <arabic text> *

      on a single line
  -->
  <xsl:template match="quote">
    <div class="arabicquoteblock">
    <table align="center" width="100%">
      <xsl:for-each select="L">
        <tr>
          <td width="10%">*</td>
          <td align="center" width="80%">
            <xsl:for-each select="foreign|orth">
              <xsl:if test="@type = 'arrow'">
                ↓
              </xsl:if>
            <span class="arabicquote">
              <xsl:value-of select="."/>
            </span>
            <xsl:if test="position() != last()">
               <span>&#160;&#160;&#160;*&#160;&#160;&#160;</span>
              </xsl:if>

            </xsl:for-each>
          </td>
          <td width="10%">*</td>
        </tr>
      </xsl:for-each>
    </table>
    </div>
  </xsl:template>

  <xsl:template match="entryFree" priority="100">

    <span>
    <xsl:choose>
      <xsl:when test="boolean(../@supp = 1)">
        <xsl:attribute name="class">entry supplement</xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="class">entry</xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
      <a>
        <xsl:attribute name="name">
          <xsl:value-of select="@id" />
        </xsl:attribute>
      </a>
    <xsl:for-each select="@*|node()">
      <!---
      <p><xsl:attribute name="pos">
        <xsl:value-of select="position()" />
      </xsl:attribute>
      -->
      <xsl:choose>
        <xsl:when test="count(.|/)=1">
          <xsl:text>Root</xsl:text>
        </xsl:when>
        <xsl:when test="self::*">
          <!---

          -->
          <xsl:choose>
            <xsl:when test="name() = 'hi'">
              <span class="hi"><xsl:value-of select="text()" /></span>
            </xsl:when>
            <xsl:when test="name() = 'form'">
              <xsl:apply-templates select="." />
            </xsl:when>
            <xsl:when test="name() = 'pb'">
              <xsl:apply-templates select="." />
            </xsl:when>
            <xsl:when test="name() = 'quote'">
              <xsl:apply-templates select="." />
            </xsl:when>
            <xsl:when test="name() = 'sense'">
              <xsl:apply-templates select="." />
            </xsl:when>
            <xsl:when test="name() = 'tropical'">
              <xsl:text>‡</xsl:text>
            </xsl:when>
            <xsl:when test="name() = 'assumedtropical'">
              <xsl:text>†</xsl:text>
            </xsl:when>
            <xsl:when test="name() = 'note'">
              <a>
                <xsl:attribute name="href">
                  <xsl:text>127.0.0.0/?text=</xsl:text>
                  <xsl:value-of select="."/>
                </xsl:attribute>
                <xsl:attribute name="class">
                  <xsl:text>systemnote</xsl:text>
                </xsl:attribute>
                <xsl:text>[Note]</xsl:text>
              </a>
            </xsl:when>
            <xsl:when test="name() = 'foreign' or 'orth'">
              <span class="arabic">
              <xsl:choose>
                <xsl:when test="@goto">
                  <a>
                    <xsl:attribute name="href">
                      <xsl:value-of select="concat('#',@nodeid)" />
                    </xsl:attribute>
                  <xsl:value-of select="text()" />
                  </a>
                </xsl:when>
                <xsl:when test="@jumptoroot">
                  <a>
                    <xsl:attribute name="href">
                      <xsl:text>127.0.0.0/?root=</xsl:text>
                      <xsl:value-of select="@jumptoroot"/>
                    </xsl:attribute>
                  <xsl:value-of select="text()" />
                  </a>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="text()" />
                </xsl:otherwise>
              </xsl:choose>
              </span>
            </xsl:when>

            <xsl:otherwise>
              <xsl:value-of select="name()"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="self::text()">
          <!---
          <xsl:text>Text</xsl:text>
          -->
          <xsl:value-of select="." />
        </xsl:when>
        <xsl:when test="self::comment()">
          <xsl:text>Comment</xsl:text>
        </xsl:when>
        <xsl:when test="self::processing-instruction()">
          <xsl:text>PI</xsl:text>
        </xsl:when>
        <xsl:when test="count(.|../@*)=count(../@*)">
          <!---
          <xsl:text>Attribute </xsl:text> <xsl:value-of select="name()" />
          -->
        </xsl:when>
      </xsl:choose>
      <!---
      </p>
      -->

    </xsl:for-each>


    </span>
      <br/>
  </xsl:template>
</xsl:stylesheet>
