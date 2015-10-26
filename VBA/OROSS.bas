Attribute VB_Name = "OROSS"
Dim tile As Boolean
Dim tileName As String
Dim orthoWait As Boolean
Dim rule As Boolean
Dim ruleNum As String

Dim objRegExpRule As Object
Dim objRegExpRuleSimp As Object
Dim objRegExpSubRule As Object

Dim footNote As Integer

Public Function ruleRegEx() As Object
    ' Create a regular expression object.
    Set objRegExp = New RegExp

    'Set the pattern by using the Pattern property.
    objRegExp.Pattern = "^(\d+)\.\s*(П\s*р\s*а\s*в\s*и\s*л\s*о\.\s*)"

    ' Set Case Insensitivity.
    objRegExp.IgnoreCase = False

    'Set global applicability.
    objRegExp.Global = True

    'Test whether the String can be compared.
     Set ruleRegEx = objRegExp
    
End Function
Public Function ruleSimpRegEx() As Object
    ' Create a regular expression object.
    Set objRegExp = New RegExp

    'Set the pattern by using the Pattern property.
    objRegExp.Pattern = "^\s*(П\sр\sа\sв\sи\sл\sо\.\s*)"

    ' Set Case Insensitivity.
    objRegExp.IgnoreCase = False

    'Set global applicability.
    objRegExp.Global = True

    'Test whether the String can be compared.
     Set ruleSimpRegEx = objRegExp
    
End Function

Public Function subRuleRegEx() As Object
    ' Create a regular expression object.
    Set objRegExp = New RegExp

    'Set the pattern by using the Pattern property.
    objRegExp.Pattern = "^\s*(\d+)\)\s*"

    ' Set Case Insensitivity.
    objRegExp.IgnoreCase = False

    'Set global applicability.
    objRegExp.Global = True

    'Test whether the String can be compared.
     Set subRuleRegEx = objRegExp
    
End Function

Sub ParseOROSS()
   
    Dim objParser As Object
    Set objParser = New OROSSParser
    res = objParser.Init()
    
    Set objRegExpRule = ruleRegEx()
    Set objRegExpRuleSimp = ruleSimpRegEx()
    Set objRegExpSubRule = subRuleRegEx()
    
    Dim theDoc As Document
    Set theDoc = Documents.Open("c:\IRYA\Справочник-test1.doc") 'c:\IRYA\Справочник 7-28 апр весь и сод.doc")

    Dim para As Paragraph
    
    Set para = theDoc.Paragraphs.item(1)
    cp = theDoc.Paragraphs.count
    
    tile = False
    orthoWait = False
    
    Call CheckFootNotes(theDoc, objParser)
    footNote = 1
'    If orthoWait Then
    
    For i = 1 To cp
        If para.Range.Characters.count > 1 Then
            Call CheckPara(para, objParser)
        End If
        Set para = para.Next()
    Next i

 '   End If
    
    Set theDoc = Documents.Open("c:\IRYA\errors.doc") '"c:\IRYA\OROS_2014 15май Романов.doc")

    Set para = theDoc.Paragraphs.item(1)
    cp = theDoc.Paragraphs.count
    
    
    For i = 1 To cp
        If Not para Is Nothing Then
            If para.Range.Characters.count > 1 Then
                Set para = CheckArticle(para, objParser)
            Else
                Set para = para.Next()
            End If
        Else
            Exit For
        End If
    Next i
    
    res = objParser.Terminate()
End Sub
Sub CheckFootNotes(theDoc As Document, oParser As Object)
    Dim note As footNote
    Dim cn As Long
    Dim idx As Long
    cn = theDoc.Range.Footnotes.count
    idx = 1
    If cn > 0 Then
        For i = 1 To cn
            Set note = theDoc.Range.Footnotes.item(i)
            text = ""
            text = text & "<sup><a href=" & Chr(34) & "#ft" & Trim$(str$(idx)) & Chr(34)
            text = text & " id=" & Chr(34) & "foot" & Trim$(str$(idx)) & Chr(34) & " >"
            text = text & "[" & Trim$(str$(idx)) & "]</a></sup>"
            
            text = text & ConvertText(note.Range.words, 1, 0, False, True)
            res = oParser.AddFootNote(idx, text)
            idx = idx + 1
        Next i
    End If
End Sub
Sub CheckPara(para As Paragraph, oParser As Object)
    Dim Num As String
    Dim prev As String
    Dim startFoot As Long
    startFoot = footNote
    
    If para.Range.Tables.count <> 0 And orthoWait Then
        Call addOrthogr(para.Range.Tables(1), oParser)
        orthoWait = False
    End If
    If IsPart(para) Then
        rule = False
        res = oParser.addPart(Mid$(para.Range.text, 1, Len(para.Range.text) - 1))
    Else
        If IsTile(para) Then
            tileName = Mid$(para.Range.text, 1, Len(para.Range.text) - 1)
            tile = True
            rule = False
        Else
            If IsPara(para) Then
                rule = False
                If tile Then
                    res = oParser.addTile(tileName)
                    tile = False
                    tileName = ""
                End If
                Num = Trim$(para.Range.words.item(2))
                If Num = "1" Then
                    Debug.Print "para 40"
                End If
                Call AddPara(Num, para, oParser)
            Else
                If tile Then
                    tileName = tileName + Mid$(para.Range.text, 1, Len(para.Range.text) - 1)
                End If
                addPrev = False
                ruleLen = IsRule(para)
                If ruleLen <> 0 Then
                    Num = Trim$(para.Range.words.item(1))
                    If Not IsDigit(Num) Then
                        prev = Trim$(para.Previous().Range.words.item(1))
                        If (IsDigit(prev)) Then
                            addPrev = True
                            Num = Trim$(para.Previous().Range.words.item(1))
                        Else
                            Num = "0"
                        End If
                    End If
                    'If Num = "П" Then
                    '    Num = "0"
                    'End If
                    ruleText = ""
                    If addPrev Then
                        ruleText = ConvertText(para.Previous().Range.words, 1, 0) & "</p></p>"
                    End If
                    ruleText = ruleText & ConvertText(para.Range.words, 1, para.Range.words.count)
                    res = oParser.addRule(CLng(Num), ruleText) 'Mid$(para.Range.text, ruleLen, Len(para.Range.text) - 1))
                    rule = True
                    ruleNum = Num
                Else
                    subLen = IsSubRule(para)
                    If rule And subLen <> 0 Then
          '              Num = Trim$(para.Range.Words.item(1))
                        ruleText = ConvertText(para.Range.words, 1, para.Range.words.count)
                        Num = Trim$(Mid$(para.Range.text, 1, subLen))
                        res = oParser.addRule(ruleNum & "." & Num, ruleText) 'Mid$(para.Range.text, ruleLen, Len(para.Range.text) - 1))
                        rule = True
                    Else
                        If IsOrthogr(para) Then
                            orthoWait = True
                            rule = False
                        Else
                            If rule Then
                                ruleText = ConvertText(para.Range.words, 1, para.Range.words.count)
                                res = oParser.AddInfoToRule(ruleText)
                            End If
                        End If
                    End If
                End If
            End If
        End If
    End If
    If startFoot <> footNote Then
        For i = startFoot To footNote - 1
            res = oParser.AddFootNote(i, "")
        Next i
    End If
End Sub

Function IsPart(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.words.first
    If Trim$(word) = "ЧАСТЬ" Then
        IsPart = True
    Else
        IsPart = False
    End If
End Function

Function IsTile(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.words.first
    IsTile = True
    For i = 1 To Len(word)
        c = Mid(word, i, 1)
        If c <> "I" And c <> "X" And c <> "V" Then
            IsTile = False
            Exit For
        End If
    Next i
End Function

Function IsPara(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.words.first
    If Trim$(word) = "§" Then
        IsPara = True
    Else
        IsPara = False
    End If
End Function

Function IsOrthogr(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.words.first
    If Trim$(word) = "Формулы" Then
        IsOrthogr = True
    Else
        IsOrthogr = False
    End If
End Function

Sub AddPara(Num As String, para As Paragraph, objParser As Object)
    Dim word As String
    Dim examples As String
    Dim name As String
    Dim count As Integer
    Done = 0
    If InStr(para.Range.text, "1)") = 0 Then
        word = Trim$(para.Range.words.item(para.Range.words.count - 1))
        If Trim$(word) = ")" Then
            comma = 0
            cl = 1
            op = 0
            count = 0 ' para.Range.Words.item(para.Range.Words.count - 1).Characters.count
            For i = para.Range.words.count - 2 To 1 Step -1
                word = Trim$(para.Range.words.item(i))
                count = count + 1 'para.Range.Words.item(i).Characters.count
                If InStr(word, ")") <> 0 Then
                    cl = cl + 1
                End If
                If InStr(word, ",") <> 0 Then
                    comma = comma + 1
                Else
                    If InStr(word, "(") <> 0 Then
                        op = op + 1
                        If cl = op Then
                            If comma > 1 Then
                                examples = ConvertText(para.Range.words, para.Range.words.count - 2 - (count - 1), para.Range.words.count - 1) 'Mid$(para.Range.text, Len(para.Range.text) - count)
                                name = ConvertText(para.Range.words, 1, para.Range.words.count - 2 - count) 'Mid$(para.Range.text, 1, Len(para.Range.text) - (count + 1))
                                res = objParser.AddPara(CLng(Num), name)
                                res = objParser.AddExamplesToPara(examples)
                            Else
                                If InStr(para.Range.text, ":") = 0 Then
                                    examples = ConvertText(para.Range.words, para.Range.words.count - 2 - (count - 1), para.Range.words.count - 1) 'Mid$(para.Range.text, Len(para.Range.text) - count)
                                    name = ConvertText(para.Range.words, 1, para.Range.words.count - 2 - count) 'Mid$(para.Range.text, 1, Len(para.Range.text) - (count + 1))
                                    res = objParser.AddPara(CLng(Num), name)
                                    res = objParser.AddExamplesToPara(examples)
                                End If
                            End If
                            Done = 1
                            Exit For
                        End If
                    End If
                End If
            Next i
        End If
    End If
    If Done = 0 Then
        name = ConvertText(para.Range.words, 1, 0)
        res = objParser.AddPara(CLng(Num), name) 'Mid$(para.Range.text, 1, Len(para.Range.text) - 1))
    End If

End Sub
Function IsRule(para As Paragraph) As Long
    Dim prev As Paragraph
    Dim text As String
    text = para.Range.text
    Dim simple As Boolean
    simple = False
    Set colMatches = objRegExpRule.Execute(text) ' Execute search.
    If colMatches.count = 0 Then
        Set colMatches = objRegExpRuleSimp.Execute(text) ' Execute search.
        simple = True
    End If
    If colMatches.count > 0 Then
        For Each objMatch In colMatches   ' Iterate Matches collection.
            IsRule = objMatch.length
            If simple Then
                Set prev = para.Previous()
                Debug.Print "Simple"
            End If
            Exit For
        Next objMatch
    Else
        IsRule = 0
    End If
End Function
Function IsSubRule(para As Paragraph) As Long
    Dim prev As Paragraph
    Dim text As String
    text = para.Range.text
    Dim simple As Boolean
    simple = False
    Set colMatches = objRegExpSubRule.Execute(text) ' Execute search.
    If colMatches.count > 0 Then
        For Each objMatch In colMatches   ' Iterate Matches collection.
            IsSubRule = objMatch.length
            Exit For
        Next objMatch
    Else
        IsSubRule = 0
    End If
End Function
Function DoReplacements(text As String) As String
    text = Replace(text, "<b><i>" & ChrW$(&HD) & ChrW$(&H7) & "</i></b>", "")
    text = Replace(text, "<b><i>" & ChrW$(&HD) & "</i></b>", "")
    text = Replace(text, "<i><u>" & ChrW$(&HD) & ChrW$(&H7) & "</u></i>", "")
    text = Replace(text, "<i><u>" & ChrW$(&HD) & "</u></i>", "")
    
    text = Replace(text, "<i>" & ChrW$(&HD) & ChrW$(&H7) & "</i>", "")
    text = Replace(text, "<i>" & ChrW$(&HD) & "</i>", "")
    
    text = Replace(text, "<b>" & ChrW$(&HD) & ChrW$(&H7) & "</b>", "")
    text = Replace(text, "<b>" & ChrW$(&HD) & "</b>", "")
    
    text = Replace(text, "<u>" & ChrW$(&HD) & ChrW$(&H7) & "</u>", "")
    text = Replace(text, "<u>" & ChrW$(&HD) & "</u>", "")

    text = Replace(text, ChrW(&HA0), " ")
    text = Replace(text, vbCrLf, "")
    text = Replace(text, ChrW(&HD), "")
    text = Replace(text, ChrW(&H7), "")
    text = Replace(text, ChrW(&H1A), "-")
'    text = Replace(text, ChrW$(&H7), "")
    For i = 1 To 3
        ch = "b"
        If i = 2 Then
            ch = "i"
        End If
        If i = 3 Then
            ch = "u"
        End If
        ' remove </b><b>, </i><i>, </u><u>
        a = "</" & ch & "><" & ch & ">"
        text = Replace(text, a, "")
        ' do not mark non-alphabetic
        '[
        text = Replace(text, "</" & ch & ">[<" & ch & ">", "[")
        ']
        text = Replace(text, "</" & ch & ">]<" & ch & ">", "]")
        '(
        text = Replace(text, "</" & ch & ">(<" & ch & ">", "(")
        ')
        text = Replace(text, "</" & ch & ">)<" & ch & ">", ")")
        
        '<b>[ => [<b>
        '<b>] => ]<b>
        text = Replace(text, "<" & ch & ">[", "[<" & ch & ">")
        text = Replace(text, "<" & ch & ">]", "]<" & ch & ">")
        ']</b> => </b>]
        '[</b> => </b>[
        text = Replace(text, "]</" & ch & ">", "</" & ch & ">]")
        text = Replace(text, "[</" & ch & ">", "</" & ch & ">[")
        '<b>( => (<b>
        ')<b> => <b>)
        text = Replace(text, "<" & ch & ">(", "(<" & ch & ">")
        text = Replace(text, ")<" & ch & ">", "<" & ch & ">)")
        '</b>) => )</b>
        '(</b> => </b>(
        text = Replace(text, "</" & ch & ">)", ")</" & ch & ">")
        text = Replace(text, "(</" & ch & ">)", "</" & ch & ">(")
        '<b>, => ,<b>
        text = Replace(text, "<" & ch & ">,", ",<" & ch & ">")
        ',</b> => </b>,
        text = Replace(text, ",</" & ch & ">", "</" & ch & ">,")
        '<b>: => :<b>
        text = Replace(text, "<" & ch & ">:", ":<" & ch & ">")
        ',</b> => </b>,
        text = Replace(text, ":</" & ch & ">", "</" & ch & ">:")
        '/
        text = Replace(text, "</" & ch & ">/<" & ch & ">", "/")
        '-
        text = Replace(text, "</" & ch & ">-<" & ch & ">", "-")
        ']/[
        text = Replace(text, "</" & ch & ">]/[<" & ch & ">", "]/[")
    
    Next i
    text = Trim$(text)
        
    DoReplacements = text
End Function

Function ConvertText(words As words, start As Integer, finish As Integer, Optional noAccent As Boolean = False, Optional noFootNote As Boolean = False) As String
    wc = words.count
    Dim length1 As Integer
    Dim length2 As Integer
    Dim text As String
    text = ""
    If finish = 0 Then
        finish = wc
    End If
    For j = start To finish
        If Len(words(j).text) > 0 Then
'            If words(j).Footnotes.count > 0 And Not noFootNote Then
'                text = text & "<sup><a href=" & Chr(34) & "#foot" & Trim$(str$(footNote)) & Chr(34)
'                text = text & " id=" & Chr(34) & "ft" & Trim$(str$(footNote)) & Chr(34) & " >"
'                text = text & "[" & Trim$(str$(footNote)) & "]</a></sup>"
'                footNote = footNote + 1
 '           Else
                If words.item(j).Bold Then
                    text = text + "<b>"
                End If
                If words.item(j).Italic Then
                    text = text + "<i>"
                End If
    '            If para.Range.Words.item(j).Underline Then
                    For i = 1 To words.item(j).Characters.count
                        If words.item(j).Characters(i).text <> " " Then
                            If words(j).Characters(i).Footnotes.count > 0 And Not noFootNote Then
                                text = text & "<sup><a href=" & Chr(34) & "#foot" & Trim$(str$(footNote)) & Chr(34)
                                text = text & " id=" & Chr(34) & "ft" & Trim$(str$(footNote)) & Chr(34) & " >"
                                text = text & "[" & Trim$(str$(footNote)) & "]</a></sup>"
                                footNote = footNote + 1
                            Else
                                If words.item(j).Characters(i) = "#" Then
                                    text = text + "&#x301"
                                Else
                                    If words.item(j).Characters(i).Underline Then
                                        text = text + "<u>" + words.item(j).Characters(i).text + "</u>"
                                    Else
                                        Code = AscW(words.item(j).Characters(i).text)
                                        text = text + words.item(j).Characters(i).text
                                    End If
                                    If Not noAccent And words.item(j).Characters(i).Font.name = "Times Roman Cyr Acsent" Then
                                        text = text + "&#x301"
                                    End If
                                End If
                            End If
                        Else
                            text = text + words.item(j).Characters(i).text
                        End If
                    Next i
     '           Else
     '               text = text + para.Range.Words.item(j).text
     '           End If
                length1 = Len(text)
                text = Trim$(text)
                length2 = Len(text)
                If words.item(j).Italic Then
                    text = text + "</i>"
                End If
                If words.item(j).Bold Then
                    text = text + "</b>"
                End If
                If (length1 > length2) Then
                    text = text & " "
                End If
        '    End If
        End If
    Next j
    
    ConvertText = DoReplacements(text)
    
End Function
Function IsDigit(Num As String) As Boolean
    digits = "0123456789"
    If Len(Num) = 0 Then
        IsDigit = False
    Else
        IsDigit = True
        For i = 1 To Len(Num)
            If InStr(digits, Mid(Num, i, 1)) = 0 Then
                IsDigit = False
                Exit For
            End If
        Next i
    End If
End Function

Sub addOrthogr(table As table, objParser As Object)
    Dim tpara As Paragraph
    cp = table.Cell(1, 1).Range.Paragraphs.count
    Set tpara = table.Cell(1, 1).Range.Paragraphs(1)
    Dim prefix As Boolean
    prefix = False
    For i = 1 To cp
        If Trim$(tpara.Range.words(1).text) = "Формулы" Then
            If tpara.Range.words.count >= 4 Then
                prefix = True
            End If
        Else
            If tpara.Range.Characters.count > 1 Then
                Call addFormula(tpara, prefix, objParser)
            End If
        End If
        Set tpara = tpara.Next()
    Next i
End Sub

Sub addFormula(para As Paragraph, prefix As Boolean, objParser As Object)
    Dim orthogr As String
    Dim formula As String
    Dim rest As String
    Dim i As Integer
    Dim j As Integer
    wc = para.Range.words.count
    orthogr = ""
    
    Dim ex As Boolean
    ex = False
    Dim rst As Boolean
    rst = False
    Dim propverka As Boolean
    proverka = False
    act = 1
    If Not para.Range.words(1).Italic And Trim$(para.Range.words(1).text) <> "(" Then
        act = 0
    End If
    dcomma = InStr(para.Range.text, ":")
    For i = 1 To wc - 1
'        If Mid$(Trim$(para.Range.Words(i).text), Len(Trim$(para.Range.Words(i).text))) = ":" Then
        If InStr(Trim$(para.Range.words(i).text), ":") Then
            orthogr = ConvertText(para.Range.words, 1, i)
            orthogr = Mid$(orthogr, 1, Len(orthogr) - 1)
            Exit For
        Else
            If dcomma = 0 Then
'                If Trim$(para.Range.Words(i).text) = "(" Then
                If InStr(Trim$(para.Range.words(i).text), "(") Then
                    If para.Range.words(i + 1).Italic And Trim$(para.Range.words(i + 1).text) <> "слово" Then
                        If Trim$(para.Range.words(i - 1).text) = ":" And (Trim$(para.Range.words(i - 2).text) = "проверка" And i - 4 > 1) Then
                            orthogr = ConvertText(para.Range.words, 1, i - 4)
                            proverka = True
                        Else
                            orthogr = ConvertText(para.Range.words, 1, i - 1)
                        End If
                        ex = True
                        Exit For
                    End If
                Else
                    If InStr(Trim$(para.Range.words(i).text), "§") Then
                        orthogr = ConvertText(para.Range.words, 1, i - 1)
                        rst = True
                        Exit For
                    End If
                End If
            End If
        End If
    Next i
    
    If proverka Then
        i = i - 3
    End If
    j = i
    If Not ex And Not rst Then
        For j = i + 1 To wc - 1
            If InStr(Trim$(para.Range.words(j).text), "§") Then
                formula = ConvertText(para.Range.words, i + 1, j - 1)
                Exit For
            Else
'                If Trim$(para.Range.Words(j).text) = "(" Then
                If InStr(Trim$(para.Range.words(j).text), "(") Then
                    If para.Range.words(j + 1).Italic And Trim$(para.Range.words(j + 1).text) <> "слово" Then
                        If Trim$(para.Range.words(j - 1).text) = ":" And (Trim$(para.Range.words(j - 2).text) = "проверка" And j - 4 > i + 1) Then
                            formula = ConvertText(para.Range.words, i + 1, j - 4)
                            proverka = True
                        Else
                          formula = ConvertText(para.Range.words, i + 1, j - 1)
                        End If
                        ex = True
                        Exit For
                    End If
                End If
            End If
        Next j
    Else
        j = i
    End If
    Dim k As Integer
    If proverka Then
        j = j - 3
    End If
    k = j
    If ex Then
        For k = j + 1 To wc - 1
            If InStr(Trim$(para.Range.words(k).text), "§") Then
                example = ConvertText(para.Range.words, j, k - 1)
                Exit For
            End If
        Next k
    End If
    rest = ConvertText(para.Range.words, k, 0)
    res = objParser.addOrthogr(orthogr, formula, example, rest, act, prefix)
    
End Sub

Function CheckArticle(para As Paragraph, oParser As Object) As Paragraph
    wc = para.Range.words.count
    Dim Article As String
    Dim Title As String
    Dim nextPara As Paragraph
    Dim i As Integer
    Dim noAccent As Boolean
    If wc > 2 Then
        For i = 1 To wc
            If Not para.Range.words(i).Bold And Not Trim$(para.Range.words(i)) = "," And Not Trim$(para.Range.words(i)) = "-" Then
                noAccent = True
                Title = ConvertText(para.Range.words, 1, i - 1, noAccent)
                Exit For
            End If
        Next i
        Article = ConvertText(para.Range.words, 1, 0)
        Set nextPara = para.Next()
        If Not nextPara Is Nothing Then
            If nextPara.Range.words(1).Characters(1) = ChrW$(&H25CA) Then '  Or Not nextPara.Range.Words(1).Bold Then
                Article = Article & "</p><p>" & ConvertText(nextPara.Range.words, 1, 0)
                Set nextPara = nextPara.Next()
            End If
            Set para = nextPara
        End If
        res = oParser.AddArticle(Title, Article)
    Else
        Set para = para.Next()
    End If
    Set CheckArticle = para
End Function

