require 'tcpserver'
require 'ds.class'
local liburl = require 'socket.url'

local table_insert = table.insert

--字符串函数
local function stringSplit(str, sep)
  local tbl = {}
  local pos = 1
  local fld
  local i,j = str:find(sep)
  while i do
    fld = str:sub(pos, i - 1)
    if #fld ~= 0 then
      table_insert(tbl, fld)
    end
    pos = j + 1
    i,j = str:find(sep, pos)
  end  
  
  fld = str:sub(pos)
  if #fld ~= 0 then
	table_insert(tbl, fld)
  end
  return tbl
end

local function stringReplace(str, old, new)
  local tar = ''
  local pos = 1
  local i,j = str:find(old)
  while i do
    fld = str:sub(pos, i - 1)
	tar = tar..fld..new
	pos = j + 1
	i,j = str:find(old, pos)
  end
  fld = str:sub(pos)
  tar = tar..fld
  return tar
end

local function stringRFind(str, pattern)
  local pos = 1
  local li,lj
  local ni,nj = str:find(pattern)
  while ni do
    li, lj = ni, nj
    pos = nj + 1
    ni, nj = str:find(pattern, pos)
  end
  return li, lj
end

Request = Class()
function Request:__init__()
  self.header = {}
  self.cookie = {}
  self.query = {}
end

Response = Class()
function Response:__init__()
  self.code = 200
  self.cookie = {}
  self.header = {}
  self.content = ''
end

WebApplication = Class(Application)
function WebApplication:handle_connect(socket)
  local thread = coroutine.create(self.run)
  coroutine.resume(thread, socket)
end

function WebApplication:__init__(rootdir)
  self.rootdir = rootdir
end

function WebApplication:run(socket)
  --最多持续30秒关闭
  local timer = add_timeout(function()
    print('timeout')
    socket.close()
  end, 30)
  local flag, err = pcall(self.serve, socket)
  if err then
    print(err)
    socket.write('HTTP/1.1 503 Internal Error.\r\nContent-Length: '..#err..'\r\n\r\n'..err)
    socket.close_on_send_complete()
  end
  del_timeout(timer)
end

function WebApplication:serve(socket)
  local request = Request()
  while true do
    local line = socket.readline()
    if #line == 0 then break end
    self.parse_line(request, line)
  end
  
  
  local contentlength = request.contentlength or 0
  if contentlength > 0 then
    request.content = socket.read(contentlength)
    assert(#request.content == contentlength)
    
    --解析post
    local posts = stringSplit(request.content, '&')
    for i, v in ipairs(posts) do
      local kv = stringSplit(v, '=')
      request.post[kv[1]] = liburl.unescape(kv[2] or '')
    end
  end
  
  local response = self.handle_request(request)
  
  --构造header
  response.header['Server'] = 'LuaServer/1.1'
  
  local header = 'HTTP/1.1 '..response.code..' OK\r\n'
  for k, v in pairs(response.header) do
    if k ~= 'Cookie' then
      header = header..k..': '..v..'\r\n'
    end
  end
  
  --构造cookie
  local cookie = ''
  for k, v in pairs(response.cookie) do
    if cookie == '' then
      cookie = k..'='..liburl.escape(v)
    else
      cookie = cookie..'; '..k..'='..liburl.escape(v)
    end
  end
  
  if #cookie > 0 then
    header = header..'Cookie: '..cookie..'\r\n'
  end
  
  if response.content and #response.content > 0 then
    header = header..'Content-Length: '..#response.content..'\r\n'
  end
  
  local content = header..'\r\n'..(response.content or '')
  socket.write(content)
  
  socket.close_on_send_complete()
end

function WebApplication:parse_line(request, line)
  if string.find(line, 'GET') == 1 or string.find(line, 'POST') == 1 then
    local fields = stringSplit(line, ' ')
    request.method = fields[1]
    request.originurl = fields[2]
    
    --去掉#后面的
    local anc = string.find(fields[2], '#')
    if anc then
      fields[2] = string.sub(fields[2], 1, anc - 1)
    end
    
    --url+?+querystring
    local urls = stringSplit(fields[2], '?')
    request.url = urls[1]
    if urls[2] then
      local queries = stringSplit(urls[2], '&')
      for i, v in ipairs(queries) do
        local kv = stringSplit(v, '=')
        request.query[kv[1]] = liburl.unescape(kv[2] or '')
      end
    end
  else
    --以: 分割
    local tokens = stringSplit(line, ': ')
    if #tokens == 2 then
      request.header[tokens[1]] = tokens[2]
      if tokens[1] == 'Cookie' then
        local cookies = stringSplit(tokens[2], '; ')
        for i, v in ipairs(cookies) do
          local kv = stringSplit(v, '=')
          request.cookie[kv[1]] = luburl.unescap(kv[2] or '')
        end
      elseif tokens[1] == 'Content-Length' then
        request.contentlength = (tokens[2] or 0) + 0
      end
    end
  end
end

function WebApplication:handle_request(request)
  local response = Response()
  response.cookie = request.cookie
  
  --错误请求
  if not request.url or #request.url == 0 then
    response.code = 500
    return response
  end
  
  --请求分发
  if not self.dispatch_request(request, response) then
    response.code = 404
  end
  return response
end

local filecache = {}

local mime = {}
mime["atom"] = "application/atom+xml";
mime["hqx"] = "application/mac-binhex40";
mime["cpt"] = "application/mac-compactpro";
mime["mathml"] = "application/mathml+xml";
mime["doc"] = "application/msword";
mime["bin"] = "application/octet-stream";
mime["dms"] = "application/octet-stream";
mime["lha"] = "application/octet-stream";
mime["lzh"] = "application/octet-stream";
mime["exe"] = "application/octet-stream";
mime["class"] = "application/octet-stream";
mime["so"] = "application/octet-stream";
mime["dll"] = "application/octet-stream";
mime["dmg"] = "application/octet-stream";
mime["oda"] = "application/oda";
mime["ogg"] = "application/ogg";
mime["pdf"] = "application/pdf";
mime["ai"] = "application/postscript";
mime["eps"] = "application/postscript";
mime["ps"] = "application/postscript";
mime["rdf"] = "application/rdf+xml";
mime["smi"] = "application/smil";
mime["smil"] = "application/smil";
mime["gram"] = "application/srgs";
mime["grxml"] = "application/srgs+xml";
mime["air"] = "application/vnd.adobe.apollo-application-installer-package+zip";
mime["mif"] = "application/vnd.mif";
mime["xul"] = "application/vnd.mozilla.xul+xml";
mime["xls"] = "application/vnd.ms-excel";
mime["ppt"] = "application/vnd.ms-powerpoint";
mime["rm"] = "application/vnd.rn-realmedia";
mime["wbxml"] = "application/vnd.wap.wbxml";
mime["wmlc"] = "application/vnd.wap.wmlc";
mime["wmlsc"] = "application/vnd.wap.wmlscriptc";
mime["vxml"] = "application/voicexml+xml";
mime["bcpio"] = "application/x-bcpio";
mime["vcd"] = "application/x-cdlink";
mime["pgn"] = "application/x-chess-pgn";
mime["cpio"] = "application/x-cpio";
mime["csh"] = "application/x-csh";
mime["dcr"] = "application/x-director";
mime["dir"] = "application/x-director";
mime["dxr"] = "application/x-director";
mime["dvi"] = "application/x-dvi";
mime["spl"] = "application/x-futuresplash";
mime["gtar"] = "application/x-gtar";
mime["hdf"] = "application/x-hdf";
mime["js"] = "application/x-javascript";
mime["latex"] = "application/x-latex";
mime["sh"] = "application/x-sh";
mime["shar"] = "application/x-shar";
mime["swf"] = "application/x-shockwave-flash";
mime["sit"] = "application/x-stuffit";
mime["sv4cpio"] = "application/x-sv4cpio";
mime["sv4crc"] = "application/x-sv4crc";
mime["tar"] = "application/x-tar";
mime["tcl"] = "application/x-tcl";
mime["tex"] = "application/x-tex";
mime["man"] = "application/x-troff-man";
mime["me"] = "application/x-troff-me";
mime["ms"] = "application/x-troff-ms";
mime["xml"] = "application/xml";
mime["xsl"] = "application/xml";
mime["xhtml"] = "application/xhtml+xml";
mime["xht"] = "application/xhtml+xml";
mime["dtd"] = "application/xml-dtd";
mime["xslt"] = "application/xslt+xml";
mime["zip"] = "application/zip";
mime["mp3"] = "audio/mpeg";
mime["mpga"] = "audio/mpeg";
mime["mp2"] = "audio/mpeg";
mime["m3u"] = "audio/x-mpegurl";
mime["wav"] = "audio/x-wav";
mime["pdb"] = "chemical/x-pdb";
mime["xyz"] = "chemical/x-xyz";
mime["bmp"] = "image/bmp";
mime["cgm"] = "image/cgm";
mime["gif"] = "image/gif";
mime["ief"] = "image/ief";
mime["jpg"] = "image/jpeg";
mime["jpeg"] = "image/jpeg";
mime["jpe"] = "image/jpeg";
mime["png"] = "image/png";
mime["svg"] = "image/svg+xml";
mime["wbmp"] = "image/vnd.wap.wbmp";
mime["ras"] = "image/x-cmu-raster";
mime["ico"] = "image/x-icon";
mime["pnm"] = "image/x-portable-anymap";
mime["pbm"] = "image/x-portable-bitmap";
mime["pgm"] = "image/x-portable-graymap";
mime["ppm"] = "image/x-portable-pixmap";
mime["rgb"] = "image/x-rgb";
mime["xbm"] = "image/x-xbitmap";
mime["xpm"] = "image/x-xpixmap";
mime["xwd"] = "image/x-xwindowdump";
mime["css"] = "text/css";
mime["html"] = "text/html";
mime["htm"] = "text/html";
mime["txt"] = "text/plain";
mime["asc"] = "text/plain";
mime["rtx"] = "text/richtext";
mime["rtf"] = "text/rtf";
mime["tsv"] = "text/tab-separated-values";
mime["wml"] = "text/vnd.wap.wml";
mime["wmls"] = "text/vnd.wap.wmlscript";
mime["etx"] = "text/x-setext";
mime["mpg"] = "video/mpeg";
mime["mpeg"] = "video/mpeg";
mime["mpe"] = "video/mpeg";
mime["flv"] = "video/x-flv";
mime["avi"] = "video/x-msvideo";
mime["movie"] = "video/x-sgi-movie";
mime['json'] = "text/plain";
mime['lua'] = "text/plain; charset=UTF-8";

function WebApplication:dispatch_request(request, response)
  print('dispatch_request '..request.url)
  
  if request.url == '/add' then
    local a = request.query.a or 0
    local b = request.query.b or 0
    response.content = 'result:'..(a + b)
    return true
  end
  
  --是否是文件
  local path = string.lower(self.rootdir..request.url)
  response.header['Content-Type'] = 'application/octet-stream'
  for i = 5, 2, -1 do
    local suffix = string.sub(request.url, -i)
    if mime[suffix] then
      response.header['Content-Type'] = mime[suffix]
      break
    end
  end
  
  if filecache[path] then
    response.content = filecache[path]
  else
    local file = io.open(path, 'rb')
    if not file then
      return
    end
    local data = file:read('*a')
    --不要传大文件
    filecache[path] = data
    response.content = data
  end
  return true
end

local json = require('json')
local decode = json.decode.decode
local function safedecode(text)
  local flag, result = pcall(decode, text)
  if flag then return result end
end

require 'luahttps'

local tokurl = 'https://graph.renren.com/oauth/token'
local appkey = '16134057c7e146d88db2e632801ae356'
local appsec = '5ba811115df44f0995bbc14187958f37'
local redurl = 'http%3a%2f%2flocalhost%3a8080%2frenren'


local sina_tokurl = 'https://api.weibo.com/oauth2/access_token'
local sina_appkey = '2393749234'
local sina_appsec = '6f27b97a0129a39ff0c2858d5c1890c9'
local sina_redurl = 'http%3a%2f%2f127.0.0.1%3a8080%2fsina'


SixRoomApplication = Class(WebApplication)

function SixRoomApplication:dispatch_request(request, response)
  local url = string.lower(request.url)
  if url == '/renren' then
    --取code
    --发起请求
    print('renren start '..request.originurl)
    
    response.header['Content-Type'] = "text/plain; charset=UTF-8"    
    
    local renrencode = request.query['code']
    if not renrencode or #renrencode == 0 then
      response.content = '认证失败'
      return
    end
    
    --取accesstoken
    local content, code = https.request(tokurl..'?grant_type=authorization_code&client_id='..appkey..'&redirect_uri='..redurl..'&client_secret='..appsec..'&code='..renrencode)
    print('renren1 '..code..'\r\n'..content)
    if code ~= 200 then
      response.content = '获取accesstoken失败'
      return true
    end
    
    local result = safedecode(content)
    if not result then
      response.content = '解析accesstoken失败'
      return true
    end
    
    --luasocket 不支持 https
    local requesturl = 'https://api.renren.com/v2/user/get?access_token='..liburl.escape(result.access_token)
    print(requesturl)
    local content, code = https.request(requesturl)
    print('renren2 '..code..'\r\n'..content)
    if code == 200 then
      local result = safedecode(content)
      if not result then
        response.content = '解析返回结果失败'
      else
        --输出个人信息
        response.content = 'ID:'..result.response.id..'\r\n'..'Name:'..result.response.name
      end
    else
      response.content = '获取消息失败'
    end
    return true
  end
  
  if url == '/sina' then
    print('sina start '..request.originurl)
    
    response.header['Content-Type'] = "text/plain; charset=UTF-8"    
    
    local renrencode = request.query['code']
    if not renrencode or #renrencode == 0 then
      response.content = '认证失败'
      return true
    end
    
    --取accesstoken
    local accessurl = sina_tokurl..'?grant_type=authorization_code&client_id='..sina_appkey..'&redirect_uri='..sina_redurl..'&client_secret='..sina_appsec..'&code='..renrencode
    print(accessurl)
    
    local content, code = https.request(accessurl, 'POST')
    print('sina1 '..code..'\r\n'..content)
    if code ~= 200 then
      response.content = '获取accesstoken失败'
      return true
    end
    
    local result = safedecode(content)
    if not result then
      response.content = '解析accesstoken失败'
      return true
    end
    
    local requesturl = 'https://api.weibo.com/2/account/get_uid.json?access_token='..liburl.escape(result.access_token)
    print(requesturl)
    local content, code = https.request(requesturl)
    print('sina2 '..code..'\r\n'..content)
    if code == 200 then
      local result = safedecode(content)
      if not result then
        response.content = '解析返回结果失败'
      else
        --输出个人信息
        response.content = 'ID:'..result.uid..'\r\n'
      end
    else
      response.content = '获取消息失败'
    end
    return true
  
  end
end

--main_loop(SixRoomApplication(), 8080)

main_loop(WebApplication('C:\\Users\\jialin\\Desktop\\luaserver'), 8080)
