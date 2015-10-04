Attribute VB_Name = "OROSS"
Dim tile As Boolean
Dim tileName As String
Dim orthoWait As Boolean

Dim objRegExpRule As Object
Dim objRegExpRuleSimp As Object
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

Sub ParseOROSS()
   
    Dim objParser As Object
    Set objParser = New OROSSParser
    res = objParser.Init()
    
    Set objRegExpRule = ruleRegEx()
    Set objRegExpRuleSimp = ruleSimpRegEx()
    
    Dim theDoc As Document
    Set theDoc = Documents.Open("c:\IRYA\Справочник 7-28 апр весь и сод.doc")

    Dim para As Paragraph
    
    Set para = theDoc.Paragraphs.item(1)
    cp = theDoc.Paragraphs.count
    
    tile = False
    orthoWait = False
    
'    If orthoWait Then
    
    For i = 1 To 0 'cp
        If para.Range.Characters.count > 1 Then
            Call CheckPara(para, objParser)
        End If
        Set para = para.Next()
    Next i

 '   End If
    
    Set theDoc = Documents.Open("c:\IRYA\errors.doc") 'c:\IRYA\OROS_2014 15май Романов.doc

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

Sub CheckPara(para As Paragraph, oParser As Object)
    Dim Num As String
    Dim prev As String
    
    If para.Range.Tables.count <> 0 And orthoWait Then
        Call addOrthogr(para.Range.Tables(1), oParser)
        orthoWait = False
    End If
    If IsPart(para) Then
        res = oParser.addPart(Mid$(para.Range.text, 1, Len(para.Range.text) - 1))
    Else
        If IsTile(para) Then
            tileName = Mid$(para.Range.text, 1, Len(para.Range.text) - 1)
            tile = True
        Else
            If IsPara(para) Then
                If tile Then
                    res = oParser.addTile(tileName)
                    tile = False
                    tileName = ""
                End If
                Num = Trim$(para.Range.Words.item(2))
                If Num = "30" Then
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
                    Num = Trim$(para.Range.Words.item(1))
                    If Not IsDigit(Num) Then
                        prev = Trim$(para.Previous().Range.Words.item(1))
                        If (IsDigit(prev)) Then
                            addPrev = True
                            Num = Trim$(para.Previous().Range.Words.item(1))
                        Else
                            Num = "0"
                        End If
                    End If
                    'If Num = "П" Then
                    '    Num = "0"
                    'End If
                    rule = ""
                    If addPrev Then
                        rule = ConvertText(para.Previous(), 1, 0) & "</p></p>"
                    End If
                    rule = rule & ConvertText(para, 1, para.Range.Words.count)
                    res = oParser.addRule(CLng(Num), rule) 'Mid$(para.Range.text, ruleLen, Len(para.Range.text) - 1))
                Else
                    If IsOrthogr(para) Then
                        orthoWait = True
                    End If
                End If
            End If
        End If
    End If
End Sub

Function IsPart(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.Words.first
    If Trim$(word) = "ЧАСТЬ" Then
        IsPart = True
    Else
        IsPart = False
    End If
End Function

Function IsTile(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.Words.first
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
    word = para.Range.Words.first
    If Trim$(word) = "§" Then
        IsPara = True
    Else
        IsPara = False
    End If
End Function

Function IsOrthogr(para As Paragraph) As Boolean
    Dim word As String
    word = para.Range.Words.first
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
        word = Trim$(para.Range.Words.item(para.Range.Words.count - 1))
        If Trim$(word) = ")" Then
            comma = 0
            cl = 1
            op = 0
            count = 0 ' para.Range.Words.item(para.Range.Words.count - 1).Characters.count
            For i = para.Range.Words.count - 2 To 1 Step -1
                word = Trim$(para.Range.Words.item(i))
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
                                examples = ConvertText(para, para.Range.Words.count - 2 - (count - 1), para.Range.Words.count - 1) 'Mid$(para.Range.text, Len(para.Range.text) - count)
                                name = ConvertText(para, 1, para.Range.Words.count - 2 - count) 'Mid$(para.Range.text, 1, Len(para.Range.text) - (count + 1))
                                res = objParser.AddPara(CLng(Num), name)
                                res = objParser.AddExamplesToPara(examples)
                            Else
                                If InStr(para.Range.text, ":") = 0 Then
                                    examples = ConvertText(para, para.Range.Words.count - 2 - (count - 1), para.Range.Words.count - 1) 'Mid$(para.Range.text, Len(para.Range.text) - count)
                                    name = ConvertText(para, 1, para.Range.Words.count - 2 - count) 'Mid$(para.Range.text, 1, Len(para.Range.text) - (count + 1))
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
        name = ConvertText(para, 1, 0)
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
Function ConvertText(para As Paragraph, start As Integer, finish As Integer) As String
    wc = para.Range.Words.count
    text = ""
    If finish = 0 Then
        finish = wc
    End If
    For j = start To finish
        If Len(para.Range.Words(j).text) > 0 Then
            If para.Range.Words.item(j).Bold Then
                text = text + "<b>"
            End If
            If para.Range.Words.item(j).Italic Then
                text = text + "<i>"
            End If
'            If para.Range.Words.item(j).Underline Then
                For i = 1 To para.Range.Words.item(j).Characters.count
                    If para.Range.Words.item(j).Characters(i).Underline Then
                        text = text + "<u>" + para.Range.Words.item(j).Characters(i).text + "</u>"
                    Else
                        Code = AscW(para.Range.Words.item(j).Characters(i).text)
                        text = text + para.Range.Words.item(j).Characters(i).text
                    End If
                Next i
 '           Else
 '               text = text + para.Range.Words.item(j).text
 '           End If
            If para.Range.Words.item(j).Italic Then
                text = text + "</i>"
            End If
            If para.Range.Words.item(j).Bold Then
                text = text + "</b>"
            End If
        End If
    Next j
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
    text = Replace(text, "</b><b>", "")
    text = Replace(text, "</i><i>", "")
    text = Replace(text, "</u><u>", "")
    text = Replace(text, "</b>/<b>", "/")
    text = Replace(text, "</i>/<i>", "/")
    text = Replace(text, "</b>[<b>", "[")
    text = Replace(text, "</b>]<b>", "]")
    text = Replace(text, "</i>[<i>", "[")
    text = Replace(text, "</i>]<i>", "]")
    text = Replace(text, "</b>(<b>", "(")
    text = Replace(text, "</b>)<b>", ")")
    text = Replace(text, "</i>(<i>", "(")
    text = Replace(text, "</i>)<i>", ")")
    
    text = Trim$(text)
    
    ConvertText = text
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
        If Trim$(tpara.Range.Words(1).text) = "Формулы" Then
            If tpara.Range.Words.count >= 4 Then
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
    wc = para.Range.Words.count
    orthogr = ""
    
    Dim ex As Boolean
    ex = False
    Dim rst As Boolean
    rst = False
    
    act = 1
    If Not para.Range.Words(1).Italic And Trim$(para.Range.Words(1).text) <> "(" Then
        act = 0
    End If
    dcomma = InStr(para.Range.text, ":")
    For i = 1 To wc - 1
'        If Mid$(Trim$(para.Range.Words(i).text), Len(Trim$(para.Range.Words(i).text))) = ":" Then
        If InStr(Trim$(para.Range.Words(i).text), ":") Then
            orthogr = ConvertText(para, 1, i)
            orthogr = Mid$(orthogr, 1, Len(orthogr) - 1)
            Exit For
        Else
            If dcomma = 0 Then
'                If Trim$(para.Range.Words(i).text) = "(" Then
                If InStr(Trim$(para.Range.Words(i).text), "(") Then
                    If para.Range.Words(i + 1).Italic And Trim$(para.Range.Words(i + 1).text) <> "слово" Then
                        orthogr = ConvertText(para, 1, i - 1)
                        ex = True
                        Exit For
                    End If
                Else
                    If InStr(Trim$(para.Range.Words(i).text), "§") Then
                        orthogr = ConvertText(para, 1, i - 1)
                        rst = True
                        Exit For
                    End If
                End If
            End If
        End If
    Next i
    
    j = i
    If Not ex And Not rst Then
        For j = i + 1 To wc - 1
            If InStr(Trim$(para.Range.Words(j).text), "§") Then
                formula = ConvertText(para, i + 1, j - 1)
                Exit For
            Else
'                If Trim$(para.Range.Words(j).text) = "(" Then
                If InStr(Trim$(para.Range.Words(j).text), "(") Then
                    If para.Range.Words(j + 1).Italic And Trim$(para.Range.Words(j + 1).text) <> "слово" Then
                        formula = ConvertText(para, i + 1, j - 1)
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
    k = j
    If ex Then
        For k = j + 1 To wc - 1
            If InStr(Trim$(para.Range.Words(k).text), "§") Then
                example = ConvertText(para, j, k - 1)
                Exit For
            End If
        Next k
    End If
    rest = ConvertText(para, k, 0)
    res = objParser.addOrthogr(orthogr, formula, example, rest, act, prefix)
    
End Sub

Function CheckArticle(para As Paragraph, oParser As Object) As Paragraph
    wc = para.Range.Words.count
    Dim Article As String
    Dim Title As String
    Dim nextPara As Paragraph
    Dim i As Integer
    If wc > 2 Then
        For i = 1 To wc
            If Not para.Range.Words(i).Bold Then
                Title = ConvertText(para, 1, i - 1)
                Exit For
            End If
        Next i
        Article = ConvertText(para, 1, 0)
        Set nextPara = para.Next()
        If Not nextPara Is Nothing Then
            If nextPara.Range.Words(1).Characters(1) = ChrW$(&H25CA) Then '  Or Not nextPara.Range.Words(1).Bold Then
                Article = Article & "</p><p>" & ConvertText(nextPara, 1, 0)
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

