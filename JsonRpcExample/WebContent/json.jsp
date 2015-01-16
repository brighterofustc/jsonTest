<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
 "http://www.w3.org/TR/html4/loose.dtd">
<%@ page contentType="text/html; charset=UTF-8"%>
<jsp:useBean id="JSONRPCBridge" scope="session" class="com.metaparadigm.jsonrpc.JSONRPCBridge" />
<jsp:useBean id="example" scope="session" class="net.vicp.jiasoft.Example" />
<%
	JSONRPCBridge.registerObject("example", example);
%>
<html>
<head>
<title>JSON-RPC-Java Example</title>
<script type="text/javascript" src="example.js"></script>
<script type="text/javascript" src="jsonrpc.js"></script>
</head>
<style>
em {
	color: red;
}
</style>
<body bgcolor="#ffffff" onLoad="onLoad()">
	<center>
		<p>
			The JSON-RPC-Java <em>Example</em> application .
		</p>
		<p>
			<strong>Who:</strong> <input type="text" id="who" size="30"
				value="brighter" /> <br /> <br /> 
				<input type="button" value="Say String" onclick="sayString()" /> 
				<input type="button" value="Say List" onclick="sayList()" /> 
				<input type="button" value="Say Map" onclick="sayMap()" /> 
				<input type="button" value="Say Set" onclick="saySet()" /> 
				<input type="button" value="Say User" onclick="sayUser()" /> 
				<input type="button" value="Put Session" onclick="putSession()" />
		</p>
		<hr />
		
		<div id="say" style="width: 520px">
			<em>返回值显示区域。</em>
			<hr />
		</div>
	</center>
</body>
</html>
