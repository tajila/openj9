/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/

#define J9_DECLARE_CONSTANT_UTF8_SIZE(index, name) \
	struct { \
		U_16 length; \
		U_8 data[sizeof(name)]; \
	} element##index

#define J9_DECLARE_CONSTANT_UTF8_VALUE(name) {sizeof(name), name},

#define J9_CONSTANT_UTF8_NEXT_ELEMENT(currentElement) ((J9UTF8 *)((U_8*)currentElement + (J9_CONSTANT_UTF8_SIZE_OF_CURRENT_ELEMENT(currentElement)) + J9_CONSTANT_UTF8_SIZE_OF_CURRENT_ELEMENT(currentElement) % sizeof(U_16)))

#define J9_CONSTANT_UTF8_SIZE_OF_CURRENT_ELEMENT(currentElement) (*((U_16 *)currentElement) + sizeof(U_16))

/* This structure keeps a list of all the packages that existed in java8 and still exist in java9.
 * In the future, once illegal-access=permit behaviour is modified to not allow any illegal access
 * from un-named modules, this list can be removed.
 *
 * Each struct member needs a unique name or the msvc compiler will complain about struct redefinition
 * error C2020.
 */
const struct {
	J9_DECLARE_CONSTANT_UTF8_SIZE(0, "com.ibm.dataaccess");
	J9_DECLARE_CONSTANT_UTF8_SIZE(1, "com.ibm.gpu");
	J9_DECLARE_CONSTANT_UTF8_SIZE(2, "com.ibm.jit.crypto");
	J9_DECLARE_CONSTANT_UTF8_SIZE(3, "com.ibm.jit");
	J9_DECLARE_CONSTANT_UTF8_SIZE(4, "com.ibm.jvm.io");
	J9_DECLARE_CONSTANT_UTF8_SIZE(5, "com.ibm.jvm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(6, "com.ibm.lang.management");
	J9_DECLARE_CONSTANT_UTF8_SIZE(7, "com.ibm.le.conditionhandling");
	J9_DECLARE_CONSTANT_UTF8_SIZE(8, "com.ibm.oti.lang");
	J9_DECLARE_CONSTANT_UTF8_SIZE(9, "com.ibm.oti.reflect");
	J9_DECLARE_CONSTANT_UTF8_SIZE(10, "com.ibm.oti.shared");
	J9_DECLARE_CONSTANT_UTF8_SIZE(11, "com.ibm.oti.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(12, "com.ibm.oti.vm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(13, "com.ibm.tools.attach.javaSE");
	J9_DECLARE_CONSTANT_UTF8_SIZE(14, "com.ibm.tools.attach.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(15, "com.ibm.tools.attach");
	J9_DECLARE_CONSTANT_UTF8_SIZE(16, "com.ibm.virtualization.management");
	J9_DECLARE_CONSTANT_UTF8_SIZE(17, "com.sun.corba.se.PortableActivationIDL.InitialNameServicePackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(18, "com.sun.corba.se.PortableActivationIDL.LocatorPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(19, "com.sun.corba.se.PortableActivationIDL.RepositoryPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(20, "com.sun.corba.se.PortableActivationIDL");
	J9_DECLARE_CONSTANT_UTF8_SIZE(21, "com.sun.corba.se.impl.dynamicany");
	J9_DECLARE_CONSTANT_UTF8_SIZE(22, "com.sun.corba.se.impl.encoding");
	J9_DECLARE_CONSTANT_UTF8_SIZE(23, "com.sun.corba.se.impl.interceptors");
	J9_DECLARE_CONSTANT_UTF8_SIZE(24, "com.sun.corba.se.impl.io");
	J9_DECLARE_CONSTANT_UTF8_SIZE(25, "com.sun.corba.se.impl.ior.iiop");
	J9_DECLARE_CONSTANT_UTF8_SIZE(26, "com.sun.corba.se.impl.ior");
	J9_DECLARE_CONSTANT_UTF8_SIZE(27, "com.sun.corba.se.impl.legacy.connection");
	J9_DECLARE_CONSTANT_UTF8_SIZE(28, "com.sun.corba.se.impl.naming.cosnaming");
	J9_DECLARE_CONSTANT_UTF8_SIZE(29, "com.sun.corba.se.impl.naming.namingutil");
	J9_DECLARE_CONSTANT_UTF8_SIZE(30, "com.sun.corba.se.impl.naming.pcosnaming");
	J9_DECLARE_CONSTANT_UTF8_SIZE(31, "com.sun.corba.se.impl.oa.poa");
	J9_DECLARE_CONSTANT_UTF8_SIZE(32, "com.sun.corba.se.impl.oa.toa");
	J9_DECLARE_CONSTANT_UTF8_SIZE(33, "com.sun.corba.se.impl.orb");
	J9_DECLARE_CONSTANT_UTF8_SIZE(34, "com.sun.corba.se.impl.orbutil.concurrent");
	J9_DECLARE_CONSTANT_UTF8_SIZE(35, "com.sun.corba.se.impl.orbutil.threadpool");
	J9_DECLARE_CONSTANT_UTF8_SIZE(36, "com.sun.corba.se.impl.orbutil");
	J9_DECLARE_CONSTANT_UTF8_SIZE(37, "com.sun.corba.se.impl.presentation.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(38, "com.sun.corba.se.impl.protocol.giopmsgheaders");
	J9_DECLARE_CONSTANT_UTF8_SIZE(39, "com.sun.corba.se.impl.protocol");
	J9_DECLARE_CONSTANT_UTF8_SIZE(40, "com.sun.corba.se.impl.resolver");
	J9_DECLARE_CONSTANT_UTF8_SIZE(41, "com.sun.corba.se.impl.transport");
	J9_DECLARE_CONSTANT_UTF8_SIZE(42, "com.sun.corba.se.internal.CosNaming");
	J9_DECLARE_CONSTANT_UTF8_SIZE(43, "com.sun.corba.se.internal.Interceptors");
	J9_DECLARE_CONSTANT_UTF8_SIZE(44, "com.sun.corba.se.internal.POA");
	J9_DECLARE_CONSTANT_UTF8_SIZE(45, "com.sun.corba.se.internal.corba");
	J9_DECLARE_CONSTANT_UTF8_SIZE(46, "com.sun.corba.se.internal.iiop");
	J9_DECLARE_CONSTANT_UTF8_SIZE(47, "com.sun.corba.se.pept.broker");
	J9_DECLARE_CONSTANT_UTF8_SIZE(48, "com.sun.corba.se.pept.encoding");
	J9_DECLARE_CONSTANT_UTF8_SIZE(49, "com.sun.corba.se.pept.protocol");
	J9_DECLARE_CONSTANT_UTF8_SIZE(50, "com.sun.corba.se.pept.transport");
	J9_DECLARE_CONSTANT_UTF8_SIZE(51, "com.sun.corba.se.spi.activation.InitialNameServicePackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(52, "com.sun.corba.se.spi.activation.LocatorPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(53, "com.sun.corba.se.spi.activation.RepositoryPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(54, "com.sun.corba.se.spi.activation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(55, "com.sun.corba.se.spi.ior.iiop");
	J9_DECLARE_CONSTANT_UTF8_SIZE(56, "com.sun.corba.se.spi.ior");
	J9_DECLARE_CONSTANT_UTF8_SIZE(57, "com.sun.corba.se.spi.legacy.connection");
	J9_DECLARE_CONSTANT_UTF8_SIZE(58, "com.sun.corba.se.spi.legacy.interceptor");
	J9_DECLARE_CONSTANT_UTF8_SIZE(59, "com.sun.corba.se.spi.monitoring");
	J9_DECLARE_CONSTANT_UTF8_SIZE(60, "com.sun.corba.se.spi.oa");
	J9_DECLARE_CONSTANT_UTF8_SIZE(61, "com.sun.corba.se.spi.orb");
	J9_DECLARE_CONSTANT_UTF8_SIZE(62, "com.sun.corba.se.spi.orbutil.fsm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(63, "com.sun.corba.se.spi.orbutil.threadpool");
	J9_DECLARE_CONSTANT_UTF8_SIZE(64, "com.sun.corba.se.spi.presentation.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(65, "com.sun.corba.se.spi.protocol");
	J9_DECLARE_CONSTANT_UTF8_SIZE(66, "com.sun.corba.se.spi.servicecontext");
	J9_DECLARE_CONSTANT_UTF8_SIZE(67, "com.sun.corba.se.spi.transport");
	J9_DECLARE_CONSTANT_UTF8_SIZE(68, "com.sun.image.codec.jpeg");
	J9_DECLARE_CONSTANT_UTF8_SIZE(69, "com.sun.imageio.plugins.bmp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(70, "com.sun.imageio.plugins.common");
	J9_DECLARE_CONSTANT_UTF8_SIZE(71, "com.sun.imageio.plugins.gif");
	J9_DECLARE_CONSTANT_UTF8_SIZE(72, "com.sun.imageio.plugins.jpeg");
	J9_DECLARE_CONSTANT_UTF8_SIZE(73, "com.sun.imageio.plugins.png");
	J9_DECLARE_CONSTANT_UTF8_SIZE(74, "com.sun.imageio.plugins.wbmp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(75, "com.sun.imageio.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(76, "com.sun.imageio.stream");
	J9_DECLARE_CONSTANT_UTF8_SIZE(77, "com.sun.java.swing.plaf.gtk.resources");
	J9_DECLARE_CONSTANT_UTF8_SIZE(78, "com.sun.java.swing.plaf.gtk");
	J9_DECLARE_CONSTANT_UTF8_SIZE(79, "com.sun.java.swing.plaf.motif.resources");
	J9_DECLARE_CONSTANT_UTF8_SIZE(80, "com.sun.java.swing.plaf.motif");
	J9_DECLARE_CONSTANT_UTF8_SIZE(81, "com.sun.java.swing.plaf.nimbus");
	J9_DECLARE_CONSTANT_UTF8_SIZE(82, "com.sun.java.swing.plaf.windows.resources");
	J9_DECLARE_CONSTANT_UTF8_SIZE(83, "com.sun.java.swing.plaf.windows");
	J9_DECLARE_CONSTANT_UTF8_SIZE(84, "com.sun.java.swing");
	J9_DECLARE_CONSTANT_UTF8_SIZE(85, "com.sun.java_cup.internal.runtime");
	J9_DECLARE_CONSTANT_UTF8_SIZE(86, "com.sun.javadoc");
	J9_DECLARE_CONSTANT_UTF8_SIZE(87, "com.sun.jmx.defaults");
	J9_DECLARE_CONSTANT_UTF8_SIZE(88, "com.sun.jmx.interceptor");
	J9_DECLARE_CONSTANT_UTF8_SIZE(89, "com.sun.jmx.mbeanserver");
	J9_DECLARE_CONSTANT_UTF8_SIZE(90, "com.sun.jmx.remote.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(91, "com.sun.jmx.remote.protocol.iiop");
	J9_DECLARE_CONSTANT_UTF8_SIZE(92, "com.sun.jmx.remote.protocol.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(93, "com.sun.jmx.remote.security");
	J9_DECLARE_CONSTANT_UTF8_SIZE(94, "com.sun.jmx.remote.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(95, "com.sun.jmx.snmp.IPAcl");
	J9_DECLARE_CONSTANT_UTF8_SIZE(96, "com.sun.jmx.snmp.agent");
	J9_DECLARE_CONSTANT_UTF8_SIZE(97, "com.sun.jmx.snmp.daemon");
	J9_DECLARE_CONSTANT_UTF8_SIZE(98, "com.sun.jmx.snmp.defaults");
	J9_DECLARE_CONSTANT_UTF8_SIZE(99, "com.sun.jmx.snmp.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(100, "com.sun.jmx.snmp.mpm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(101, "com.sun.jmx.snmp.tasks");
	J9_DECLARE_CONSTANT_UTF8_SIZE(102, "com.sun.jmx.snmp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(103, "com.sun.naming.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(104, "com.sun.org.apache.bcel.internal.classfile");
	J9_DECLARE_CONSTANT_UTF8_SIZE(105, "com.sun.org.apache.bcel.internal.generic");
	J9_DECLARE_CONSTANT_UTF8_SIZE(106, "com.sun.org.apache.bcel.internal.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(107, "com.sun.org.apache.bcel.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(108, "com.sun.org.apache.regexp.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(109, "com.sun.org.apache.xalan.internal.extensions");
	J9_DECLARE_CONSTANT_UTF8_SIZE(110, "com.sun.org.apache.xalan.internal.lib");
	J9_DECLARE_CONSTANT_UTF8_SIZE(111, "com.sun.org.apache.xalan.internal.res");
	J9_DECLARE_CONSTANT_UTF8_SIZE(112, "com.sun.org.apache.xalan.internal.templates");
	J9_DECLARE_CONSTANT_UTF8_SIZE(113, "com.sun.org.apache.xalan.internal.utils");
	J9_DECLARE_CONSTANT_UTF8_SIZE(114, "com.sun.org.apache.xalan.internal.xslt");
	J9_DECLARE_CONSTANT_UTF8_SIZE(115, "com.sun.org.apache.xalan.internal.xsltc.cmdline.getopt");
	J9_DECLARE_CONSTANT_UTF8_SIZE(116, "com.sun.org.apache.xalan.internal.xsltc.cmdline");
	J9_DECLARE_CONSTANT_UTF8_SIZE(117, "com.sun.org.apache.xalan.internal.xsltc.compiler.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(118, "com.sun.org.apache.xalan.internal.xsltc.compiler");
	J9_DECLARE_CONSTANT_UTF8_SIZE(119, "com.sun.org.apache.xalan.internal.xsltc.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(120, "com.sun.org.apache.xalan.internal.xsltc.runtime.output");
	J9_DECLARE_CONSTANT_UTF8_SIZE(121, "com.sun.org.apache.xalan.internal.xsltc.runtime");
	J9_DECLARE_CONSTANT_UTF8_SIZE(122, "com.sun.org.apache.xalan.internal.xsltc.trax");
	J9_DECLARE_CONSTANT_UTF8_SIZE(123, "com.sun.org.apache.xalan.internal.xsltc.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(124, "com.sun.org.apache.xalan.internal.xsltc");
	J9_DECLARE_CONSTANT_UTF8_SIZE(125, "com.sun.org.apache.xalan.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(126, "com.sun.org.apache.xerces.internal.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(127, "com.sun.org.apache.xerces.internal.impl.dtd.models");
	J9_DECLARE_CONSTANT_UTF8_SIZE(128, "com.sun.org.apache.xerces.internal.impl.dtd");
	J9_DECLARE_CONSTANT_UTF8_SIZE(129, "com.sun.org.apache.xerces.internal.impl.dv.dtd");
	J9_DECLARE_CONSTANT_UTF8_SIZE(130, "com.sun.org.apache.xerces.internal.impl.dv.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(131, "com.sun.org.apache.xerces.internal.impl.dv.xs");
	J9_DECLARE_CONSTANT_UTF8_SIZE(132, "com.sun.org.apache.xerces.internal.impl.dv");
	J9_DECLARE_CONSTANT_UTF8_SIZE(133, "com.sun.org.apache.xerces.internal.impl.io");
	J9_DECLARE_CONSTANT_UTF8_SIZE(134, "com.sun.org.apache.xerces.internal.impl.msg");
	J9_DECLARE_CONSTANT_UTF8_SIZE(135, "com.sun.org.apache.xerces.internal.impl.validation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(136, "com.sun.org.apache.xerces.internal.impl.xpath.regex");
	J9_DECLARE_CONSTANT_UTF8_SIZE(137, "com.sun.org.apache.xerces.internal.impl.xpath");
	J9_DECLARE_CONSTANT_UTF8_SIZE(138, "com.sun.org.apache.xerces.internal.impl.xs.identity");
	J9_DECLARE_CONSTANT_UTF8_SIZE(139, "com.sun.org.apache.xerces.internal.impl.xs.models");
	J9_DECLARE_CONSTANT_UTF8_SIZE(140, "com.sun.org.apache.xerces.internal.impl.xs.opti");
	J9_DECLARE_CONSTANT_UTF8_SIZE(141, "com.sun.org.apache.xerces.internal.impl.xs.traversers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(142, "com.sun.org.apache.xerces.internal.impl.xs.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(143, "com.sun.org.apache.xerces.internal.impl.xs");
	J9_DECLARE_CONSTANT_UTF8_SIZE(144, "com.sun.org.apache.xerces.internal.impl");
	J9_DECLARE_CONSTANT_UTF8_SIZE(145, "com.sun.org.apache.xerces.internal.jaxp.datatype");
	J9_DECLARE_CONSTANT_UTF8_SIZE(146, "com.sun.org.apache.xerces.internal.jaxp.validation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(147, "com.sun.org.apache.xerces.internal.jaxp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(148, "com.sun.org.apache.xerces.internal.parsers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(149, "com.sun.org.apache.xerces.internal.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(150, "com.sun.org.apache.xerces.internal.utils");
	J9_DECLARE_CONSTANT_UTF8_SIZE(151, "com.sun.org.apache.xerces.internal.xinclude");
	J9_DECLARE_CONSTANT_UTF8_SIZE(152, "com.sun.org.apache.xerces.internal.xni.grammars");
	J9_DECLARE_CONSTANT_UTF8_SIZE(153, "com.sun.org.apache.xerces.internal.xni.parser");
	J9_DECLARE_CONSTANT_UTF8_SIZE(154, "com.sun.org.apache.xerces.internal.xni");
	J9_DECLARE_CONSTANT_UTF8_SIZE(155, "com.sun.org.apache.xerces.internal.xpointer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(156, "com.sun.org.apache.xerces.internal.xs.datatypes");
	J9_DECLARE_CONSTANT_UTF8_SIZE(157, "com.sun.org.apache.xerces.internal.xs");
	J9_DECLARE_CONSTANT_UTF8_SIZE(158, "com.sun.org.apache.xml.internal.dtm.ref.dom2dtm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(159, "com.sun.org.apache.xml.internal.dtm.ref.sax2dtm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(160, "com.sun.org.apache.xml.internal.dtm.ref");
	J9_DECLARE_CONSTANT_UTF8_SIZE(161, "com.sun.org.apache.xml.internal.dtm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(162, "com.sun.org.apache.xml.internal.res");
	J9_DECLARE_CONSTANT_UTF8_SIZE(163, "com.sun.org.apache.xml.internal.resolver.helpers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(164, "com.sun.org.apache.xml.internal.resolver.readers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(165, "com.sun.org.apache.xml.internal.resolver.tools");
	J9_DECLARE_CONSTANT_UTF8_SIZE(166, "com.sun.org.apache.xml.internal.resolver");
	J9_DECLARE_CONSTANT_UTF8_SIZE(167, "com.sun.org.apache.xml.internal.security.algorithms.implementations");
	J9_DECLARE_CONSTANT_UTF8_SIZE(168, "com.sun.org.apache.xml.internal.security.algorithms");
	J9_DECLARE_CONSTANT_UTF8_SIZE(169, "com.sun.org.apache.xml.internal.security.c14n.helper");
	J9_DECLARE_CONSTANT_UTF8_SIZE(170, "com.sun.org.apache.xml.internal.security.c14n.implementations");
	J9_DECLARE_CONSTANT_UTF8_SIZE(171, "com.sun.org.apache.xml.internal.security.c14n");
	J9_DECLARE_CONSTANT_UTF8_SIZE(172, "com.sun.org.apache.xml.internal.security.encryption");
	J9_DECLARE_CONSTANT_UTF8_SIZE(173, "com.sun.org.apache.xml.internal.security.exceptions");
	J9_DECLARE_CONSTANT_UTF8_SIZE(174, "com.sun.org.apache.xml.internal.security.keys.keyresolver.implementations");
	J9_DECLARE_CONSTANT_UTF8_SIZE(175, "com.sun.org.apache.xml.internal.security.keys.keyresolver");
	J9_DECLARE_CONSTANT_UTF8_SIZE(176, "com.sun.org.apache.xml.internal.security.keys");
	J9_DECLARE_CONSTANT_UTF8_SIZE(177, "com.sun.org.apache.xml.internal.security.signature.reference");
	J9_DECLARE_CONSTANT_UTF8_SIZE(178, "com.sun.org.apache.xml.internal.security.signature");
	J9_DECLARE_CONSTANT_UTF8_SIZE(179, "com.sun.org.apache.xml.internal.security.transforms.implementations");
	J9_DECLARE_CONSTANT_UTF8_SIZE(180, "com.sun.org.apache.xml.internal.security.transforms.params");
	J9_DECLARE_CONSTANT_UTF8_SIZE(181, "com.sun.org.apache.xml.internal.security.transforms");
	J9_DECLARE_CONSTANT_UTF8_SIZE(182, "com.sun.org.apache.xml.internal.security.utils");
	J9_DECLARE_CONSTANT_UTF8_SIZE(183, "com.sun.org.apache.xml.internal.security");
	J9_DECLARE_CONSTANT_UTF8_SIZE(184, "com.sun.org.apache.xml.internal.serialize");
	J9_DECLARE_CONSTANT_UTF8_SIZE(185, "com.sun.org.apache.xml.internal.serializer.utils");
	J9_DECLARE_CONSTANT_UTF8_SIZE(186, "com.sun.org.apache.xml.internal.serializer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(187, "com.sun.org.apache.xml.internal.utils.res");
	J9_DECLARE_CONSTANT_UTF8_SIZE(188, "com.sun.org.apache.xml.internal.utils");
	J9_DECLARE_CONSTANT_UTF8_SIZE(189, "com.sun.org.apache.xpath.internal.axes");
	J9_DECLARE_CONSTANT_UTF8_SIZE(190, "com.sun.org.apache.xpath.internal.compiler");
	J9_DECLARE_CONSTANT_UTF8_SIZE(191, "com.sun.org.apache.xpath.internal.domapi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(192, "com.sun.org.apache.xpath.internal.functions");
	J9_DECLARE_CONSTANT_UTF8_SIZE(193, "com.sun.org.apache.xpath.internal.jaxp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(194, "com.sun.org.apache.xpath.internal.objects");
	J9_DECLARE_CONSTANT_UTF8_SIZE(195, "com.sun.org.apache.xpath.internal.operations");
	J9_DECLARE_CONSTANT_UTF8_SIZE(196, "com.sun.org.apache.xpath.internal.patterns");
	J9_DECLARE_CONSTANT_UTF8_SIZE(197, "com.sun.org.apache.xpath.internal.res");
	J9_DECLARE_CONSTANT_UTF8_SIZE(198, "com.sun.org.apache.xpath.internal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(199, "com.sun.security.auth.callback");
	J9_DECLARE_CONSTANT_UTF8_SIZE(200, "com.sun.security.auth.login");
	J9_DECLARE_CONSTANT_UTF8_SIZE(201, "com.sun.security.auth.module");
	J9_DECLARE_CONSTANT_UTF8_SIZE(202, "com.sun.security.auth");
	J9_DECLARE_CONSTANT_UTF8_SIZE(203, "com.sun.security.jgss");
	J9_DECLARE_CONSTANT_UTF8_SIZE(204, "com.sun.source.doctree");
	J9_DECLARE_CONSTANT_UTF8_SIZE(205, "com.sun.source.tree");
	J9_DECLARE_CONSTANT_UTF8_SIZE(206, "com.sun.source.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(207, "com.x.logging");
	J9_DECLARE_CONSTANT_UTF8_SIZE(208, "java.applet");
	J9_DECLARE_CONSTANT_UTF8_SIZE(209, "java.awt.color");
	J9_DECLARE_CONSTANT_UTF8_SIZE(210, "java.awt.datatransfer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(211, "java.awt.dnd.peer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(212, "java.awt.dnd");
	J9_DECLARE_CONSTANT_UTF8_SIZE(213, "java.awt.event");
	J9_DECLARE_CONSTANT_UTF8_SIZE(214, "java.awt.font");
	J9_DECLARE_CONSTANT_UTF8_SIZE(215, "java.awt.geom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(216, "java.awt.im.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(217, "java.awt.im");
	J9_DECLARE_CONSTANT_UTF8_SIZE(218, "java.awt.image.renderable");
	J9_DECLARE_CONSTANT_UTF8_SIZE(219, "java.awt.image");
	J9_DECLARE_CONSTANT_UTF8_SIZE(220, "java.awt.peer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(221, "java.awt.print");
	J9_DECLARE_CONSTANT_UTF8_SIZE(222, "java.awt");
	J9_DECLARE_CONSTANT_UTF8_SIZE(223, "java.beans.beancontext");
	J9_DECLARE_CONSTANT_UTF8_SIZE(224, "java.beans");
	J9_DECLARE_CONSTANT_UTF8_SIZE(225, "java.io");
	J9_DECLARE_CONSTANT_UTF8_SIZE(226, "java.lang.annotation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(227, "java.lang.instrument");
	J9_DECLARE_CONSTANT_UTF8_SIZE(228, "java.lang.invoke");
	J9_DECLARE_CONSTANT_UTF8_SIZE(229, "java.lang.management");
	J9_DECLARE_CONSTANT_UTF8_SIZE(230, "java.lang.ref");
	J9_DECLARE_CONSTANT_UTF8_SIZE(231, "java.lang.reflect");
	J9_DECLARE_CONSTANT_UTF8_SIZE(232, "java.lang");
	J9_DECLARE_CONSTANT_UTF8_SIZE(233, "java.math");
	J9_DECLARE_CONSTANT_UTF8_SIZE(234, "java.net");
	J9_DECLARE_CONSTANT_UTF8_SIZE(235, "java.nio.channels.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(236, "java.nio.channels");
	J9_DECLARE_CONSTANT_UTF8_SIZE(237, "java.nio.charset.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(238, "java.nio.charset");
	J9_DECLARE_CONSTANT_UTF8_SIZE(239, "java.nio.file.attribute");
	J9_DECLARE_CONSTANT_UTF8_SIZE(240, "java.nio.file.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(241, "java.nio.file");
	J9_DECLARE_CONSTANT_UTF8_SIZE(242, "java.nio");
	J9_DECLARE_CONSTANT_UTF8_SIZE(243, "java.rmi.activation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(244, "java.rmi.dgc");
	J9_DECLARE_CONSTANT_UTF8_SIZE(245, "java.rmi.registry");
	J9_DECLARE_CONSTANT_UTF8_SIZE(246, "java.rmi.server");
	J9_DECLARE_CONSTANT_UTF8_SIZE(247, "java.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(248, "java.security.acl");
	J9_DECLARE_CONSTANT_UTF8_SIZE(249, "java.security.cert");
	J9_DECLARE_CONSTANT_UTF8_SIZE(250, "java.security.interfaces");
	J9_DECLARE_CONSTANT_UTF8_SIZE(251, "java.security.spec");
	J9_DECLARE_CONSTANT_UTF8_SIZE(252, "java.security");
	J9_DECLARE_CONSTANT_UTF8_SIZE(253, "java.sql");
	J9_DECLARE_CONSTANT_UTF8_SIZE(254, "java.text.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(255, "java.text");
	J9_DECLARE_CONSTANT_UTF8_SIZE(256, "java.time.chrono");
	J9_DECLARE_CONSTANT_UTF8_SIZE(257, "java.time.format");
	J9_DECLARE_CONSTANT_UTF8_SIZE(258, "java.time.temporal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(259, "java.time.zone");
	J9_DECLARE_CONSTANT_UTF8_SIZE(260, "java.time");
	J9_DECLARE_CONSTANT_UTF8_SIZE(261, "java.util.concurrent.atomic");
	J9_DECLARE_CONSTANT_UTF8_SIZE(262, "java.util.concurrent.locks");
	J9_DECLARE_CONSTANT_UTF8_SIZE(263, "java.util.concurrent");
	J9_DECLARE_CONSTANT_UTF8_SIZE(264, "java.util.function");
	J9_DECLARE_CONSTANT_UTF8_SIZE(265, "java.util.jar");
	J9_DECLARE_CONSTANT_UTF8_SIZE(266, "java.util.logging");
	J9_DECLARE_CONSTANT_UTF8_SIZE(267, "java.util.prefs");
	J9_DECLARE_CONSTANT_UTF8_SIZE(268, "java.util.regex");
	J9_DECLARE_CONSTANT_UTF8_SIZE(269, "java.util.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(270, "java.util.stream");
	J9_DECLARE_CONSTANT_UTF8_SIZE(271, "java.util.zip");
	J9_DECLARE_CONSTANT_UTF8_SIZE(272, "java.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(273, "javax.accessibility");
	J9_DECLARE_CONSTANT_UTF8_SIZE(274, "javax.annotation.processing");
	J9_DECLARE_CONSTANT_UTF8_SIZE(275, "javax.annotation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(276, "javax.imageio.event");
	J9_DECLARE_CONSTANT_UTF8_SIZE(277, "javax.imageio.metadata");
	J9_DECLARE_CONSTANT_UTF8_SIZE(278, "javax.imageio.plugins.bmp");
	J9_DECLARE_CONSTANT_UTF8_SIZE(279, "javax.imageio.plugins.jpeg");
	J9_DECLARE_CONSTANT_UTF8_SIZE(280, "javax.imageio.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(281, "javax.imageio.stream");
	J9_DECLARE_CONSTANT_UTF8_SIZE(282, "javax.imageio");
	J9_DECLARE_CONSTANT_UTF8_SIZE(283, "javax.lang.model.element");
	J9_DECLARE_CONSTANT_UTF8_SIZE(284, "javax.lang.model.type");
	J9_DECLARE_CONSTANT_UTF8_SIZE(285, "javax.lang.model.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(286, "javax.lang.model");
	J9_DECLARE_CONSTANT_UTF8_SIZE(287, "javax.management.loading");
	J9_DECLARE_CONSTANT_UTF8_SIZE(288, "javax.management.modelmbean");
	J9_DECLARE_CONSTANT_UTF8_SIZE(289, "javax.management.monitor");
	J9_DECLARE_CONSTANT_UTF8_SIZE(290, "javax.management.openmbean");
	J9_DECLARE_CONSTANT_UTF8_SIZE(291, "javax.management.relation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(292, "javax.management.remote.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(293, "javax.management.remote");
	J9_DECLARE_CONSTANT_UTF8_SIZE(294, "javax.management.timer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(295, "javax.management");
	J9_DECLARE_CONSTANT_UTF8_SIZE(296, "javax.naming.directory");
	J9_DECLARE_CONSTANT_UTF8_SIZE(297, "javax.naming.event");
	J9_DECLARE_CONSTANT_UTF8_SIZE(298, "javax.naming.ldap");
	J9_DECLARE_CONSTANT_UTF8_SIZE(299, "javax.naming.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(300, "javax.naming");
	J9_DECLARE_CONSTANT_UTF8_SIZE(301, "javax.print.attribute.standard");
	J9_DECLARE_CONSTANT_UTF8_SIZE(302, "javax.print.attribute");
	J9_DECLARE_CONSTANT_UTF8_SIZE(303, "javax.print.event");
	J9_DECLARE_CONSTANT_UTF8_SIZE(304, "javax.print");
	J9_DECLARE_CONSTANT_UTF8_SIZE(305, "javax.rcm.management");
	J9_DECLARE_CONSTANT_UTF8_SIZE(306, "javax.rcm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(307, "javax.rmi.CORBA");
	J9_DECLARE_CONSTANT_UTF8_SIZE(308, "javax.rmi.ssl");
	J9_DECLARE_CONSTANT_UTF8_SIZE(309, "javax.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(310, "javax.script");
	J9_DECLARE_CONSTANT_UTF8_SIZE(311, "javax.security.auth.callback");
	J9_DECLARE_CONSTANT_UTF8_SIZE(312, "javax.security.auth.kerberos");
	J9_DECLARE_CONSTANT_UTF8_SIZE(313, "javax.security.auth.login");
	J9_DECLARE_CONSTANT_UTF8_SIZE(314, "javax.security.auth.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(315, "javax.security.auth.x500");
	J9_DECLARE_CONSTANT_UTF8_SIZE(316, "javax.security.auth");
	J9_DECLARE_CONSTANT_UTF8_SIZE(317, "javax.security.cert");
	J9_DECLARE_CONSTANT_UTF8_SIZE(318, "javax.security.sasl");
	J9_DECLARE_CONSTANT_UTF8_SIZE(319, "javax.sound.midi.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(320, "javax.sound.midi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(321, "javax.sound.sampled.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(322, "javax.sound.sampled");
	J9_DECLARE_CONSTANT_UTF8_SIZE(323, "javax.sql.rowset.serial");
	J9_DECLARE_CONSTANT_UTF8_SIZE(324, "javax.sql.rowset.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(325, "javax.sql.rowset");
	J9_DECLARE_CONSTANT_UTF8_SIZE(326, "javax.sql");
	J9_DECLARE_CONSTANT_UTF8_SIZE(327, "javax.swing.border");
	J9_DECLARE_CONSTANT_UTF8_SIZE(328, "javax.swing.colorchooser");
	J9_DECLARE_CONSTANT_UTF8_SIZE(329, "javax.swing.event");
	J9_DECLARE_CONSTANT_UTF8_SIZE(330, "javax.swing.filechooser");
	J9_DECLARE_CONSTANT_UTF8_SIZE(331, "javax.swing.plaf.basic");
	J9_DECLARE_CONSTANT_UTF8_SIZE(332, "javax.swing.plaf.metal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(333, "javax.swing.plaf.multi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(334, "javax.swing.plaf.nimbus");
	J9_DECLARE_CONSTANT_UTF8_SIZE(335, "javax.swing.plaf.synth");
	J9_DECLARE_CONSTANT_UTF8_SIZE(336, "javax.swing.plaf");
	J9_DECLARE_CONSTANT_UTF8_SIZE(337, "javax.swing.table");
	J9_DECLARE_CONSTANT_UTF8_SIZE(338, "javax.swing.text.html.parser");
	J9_DECLARE_CONSTANT_UTF8_SIZE(339, "javax.swing.text.html");
	J9_DECLARE_CONSTANT_UTF8_SIZE(340, "javax.swing.text.rtf");
	J9_DECLARE_CONSTANT_UTF8_SIZE(341, "javax.swing.text");
	J9_DECLARE_CONSTANT_UTF8_SIZE(342, "javax.swing.tree");
	J9_DECLARE_CONSTANT_UTF8_SIZE(343, "javax.swing.undo");
	J9_DECLARE_CONSTANT_UTF8_SIZE(344, "javax.swing");
	J9_DECLARE_CONSTANT_UTF8_SIZE(345, "javax.tools");
	J9_DECLARE_CONSTANT_UTF8_SIZE(346, "javax.xml.bind.annotation.adapters");
	J9_DECLARE_CONSTANT_UTF8_SIZE(347, "javax.xml.bind.annotation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(348, "javax.xml.bind.attachment");
	J9_DECLARE_CONSTANT_UTF8_SIZE(349, "javax.xml.bind.helpers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(350, "javax.xml.bind.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(351, "javax.xml.bind");
	J9_DECLARE_CONSTANT_UTF8_SIZE(352, "javax.xml.crypto.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(353, "javax.xml.crypto.dsig.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(354, "javax.xml.crypto.dsig.keyinfo");
	J9_DECLARE_CONSTANT_UTF8_SIZE(355, "javax.xml.crypto.dsig.spec");
	J9_DECLARE_CONSTANT_UTF8_SIZE(356, "javax.xml.crypto.dsig");
	J9_DECLARE_CONSTANT_UTF8_SIZE(357, "javax.xml.crypto");
	J9_DECLARE_CONSTANT_UTF8_SIZE(358, "javax.xml.datatype");
	J9_DECLARE_CONSTANT_UTF8_SIZE(359, "javax.xml.namespace");
	J9_DECLARE_CONSTANT_UTF8_SIZE(360, "javax.xml.parsers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(361, "javax.xml.soap");
	J9_DECLARE_CONSTANT_UTF8_SIZE(362, "javax.xml.stream.events");
	J9_DECLARE_CONSTANT_UTF8_SIZE(363, "javax.xml.stream.util");
	J9_DECLARE_CONSTANT_UTF8_SIZE(364, "javax.xml.stream");
	J9_DECLARE_CONSTANT_UTF8_SIZE(365, "javax.xml.transform.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(366, "javax.xml.transform.sax");
	J9_DECLARE_CONSTANT_UTF8_SIZE(367, "javax.xml.transform.stax");
	J9_DECLARE_CONSTANT_UTF8_SIZE(368, "javax.xml.transform.stream");
	J9_DECLARE_CONSTANT_UTF8_SIZE(369, "javax.xml.transform");
	J9_DECLARE_CONSTANT_UTF8_SIZE(370, "javax.xml.validation");
	J9_DECLARE_CONSTANT_UTF8_SIZE(371, "javax.xml.ws.handler.soap");
	J9_DECLARE_CONSTANT_UTF8_SIZE(372, "javax.xml.ws.handler");
	J9_DECLARE_CONSTANT_UTF8_SIZE(373, "javax.xml.ws.http");
	J9_DECLARE_CONSTANT_UTF8_SIZE(374, "javax.xml.ws.soap");
	J9_DECLARE_CONSTANT_UTF8_SIZE(375, "javax.xml.ws.spi.http");
	J9_DECLARE_CONSTANT_UTF8_SIZE(376, "javax.xml.ws.spi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(377, "javax.xml.ws.wsaddressing");
	J9_DECLARE_CONSTANT_UTF8_SIZE(378, "javax.xml.ws");
	J9_DECLARE_CONSTANT_UTF8_SIZE(379, "javax.xml.xpath");
	J9_DECLARE_CONSTANT_UTF8_SIZE(380, "javax.xml");
	J9_DECLARE_CONSTANT_UTF8_SIZE(381, "org.apache.harmony.kernel.vm");
	J9_DECLARE_CONSTANT_UTF8_SIZE(382, "org.ietf.jgss");
	J9_DECLARE_CONSTANT_UTF8_SIZE(383, "org.omg.CORBA.DynAnyPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(384, "org.omg.CORBA.ORBPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(385, "org.omg.CORBA.TypeCodePackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(386, "org.omg.CORBA.portable");
	J9_DECLARE_CONSTANT_UTF8_SIZE(387, "org.omg.CORBA");
	J9_DECLARE_CONSTANT_UTF8_SIZE(388, "org.omg.CORBA_2_3.portable");
	J9_DECLARE_CONSTANT_UTF8_SIZE(389, "org.omg.CORBA_2_3");
	J9_DECLARE_CONSTANT_UTF8_SIZE(390, "org.omg.CosNaming.NamingContextExtPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(391, "org.omg.CosNaming.NamingContextPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(392, "org.omg.CosNaming");
	J9_DECLARE_CONSTANT_UTF8_SIZE(393, "org.omg.Dynamic");
	J9_DECLARE_CONSTANT_UTF8_SIZE(394, "org.omg.DynamicAny.DynAnyFactoryPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(395, "org.omg.DynamicAny.DynAnyPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(396, "org.omg.DynamicAny");
	J9_DECLARE_CONSTANT_UTF8_SIZE(397, "org.omg.IOP.CodecFactoryPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(398, "org.omg.IOP.CodecPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(399, "org.omg.IOP");
	J9_DECLARE_CONSTANT_UTF8_SIZE(400, "org.omg.Messaging");
	J9_DECLARE_CONSTANT_UTF8_SIZE(401, "org.omg.PortableInterceptor.ORBInitInfoPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(402, "org.omg.PortableInterceptor");
	J9_DECLARE_CONSTANT_UTF8_SIZE(403, "org.omg.PortableServer.CurrentPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(404, "org.omg.PortableServer.POAManagerPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(405, "org.omg.PortableServer.POAPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(406, "org.omg.PortableServer.ServantLocatorPackage");
	J9_DECLARE_CONSTANT_UTF8_SIZE(407, "org.omg.PortableServer.portable");
	J9_DECLARE_CONSTANT_UTF8_SIZE(408, "org.omg.PortableServer");
	J9_DECLARE_CONSTANT_UTF8_SIZE(409, "org.omg.SendingContext");
	J9_DECLARE_CONSTANT_UTF8_SIZE(410, "org.omg.stub.java.rmi");
	J9_DECLARE_CONSTANT_UTF8_SIZE(411, "org.w3c.dom.bootstrap");
	J9_DECLARE_CONSTANT_UTF8_SIZE(412, "org.w3c.dom.css");
	J9_DECLARE_CONSTANT_UTF8_SIZE(413, "org.w3c.dom.events");
	J9_DECLARE_CONSTANT_UTF8_SIZE(414, "org.w3c.dom.html");
	J9_DECLARE_CONSTANT_UTF8_SIZE(415, "org.w3c.dom.ls");
	J9_DECLARE_CONSTANT_UTF8_SIZE(416, "org.w3c.dom.ranges");
	J9_DECLARE_CONSTANT_UTF8_SIZE(417, "org.w3c.dom.stylesheets");
	J9_DECLARE_CONSTANT_UTF8_SIZE(418, "org.w3c.dom.traversal");
	J9_DECLARE_CONSTANT_UTF8_SIZE(419, "org.w3c.dom.views");
	J9_DECLARE_CONSTANT_UTF8_SIZE(420, "org.w3c.dom.xpath");
	J9_DECLARE_CONSTANT_UTF8_SIZE(421, "org.w3c.dom");
	J9_DECLARE_CONSTANT_UTF8_SIZE(422, "org.xml.sax.ext");
	J9_DECLARE_CONSTANT_UTF8_SIZE(423, "org.xml.sax.helpers");
	J9_DECLARE_CONSTANT_UTF8_SIZE(424, "org.xml.sax");
	J9_DECLARE_CONSTANT_UTF8_SIZE(425, "sun.reflect");
} java8packages = {
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.dataaccess")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.gpu")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.jit.crypto")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.jit")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.jvm.io")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.jvm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.lang.management")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.le.conditionhandling")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.oti.lang")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.oti.reflect")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.oti.shared")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.oti.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.oti.vm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.tools.attach.javaSE")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.tools.attach.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.tools.attach")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.ibm.virtualization.management")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.PortableActivationIDL.InitialNameServicePackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.PortableActivationIDL.LocatorPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.PortableActivationIDL.RepositoryPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.PortableActivationIDL")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.dynamicany")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.encoding")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.interceptors")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.io")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.ior.iiop")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.ior")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.legacy.connection")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.naming.cosnaming")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.naming.namingutil")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.naming.pcosnaming")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.oa.poa")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.oa.toa")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.orb")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.orbutil.concurrent")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.orbutil.threadpool")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.orbutil")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.presentation.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.protocol.giopmsgheaders")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.protocol")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.resolver")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.impl.transport")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.internal.CosNaming")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.internal.Interceptors")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.internal.POA")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.internal.corba")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.internal.iiop")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.pept.broker")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.pept.encoding")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.pept.protocol")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.pept.transport")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.activation.InitialNameServicePackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.activation.LocatorPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.activation.RepositoryPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.activation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.ior.iiop")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.ior")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.legacy.connection")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.legacy.interceptor")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.monitoring")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.oa")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.orb")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.orbutil.fsm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.orbutil.threadpool")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.presentation.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.protocol")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.servicecontext")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.corba.se.spi.transport")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.image.codec.jpeg")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.bmp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.common")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.gif")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.jpeg")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.png")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.plugins.wbmp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.imageio.stream")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.gtk.resources")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.gtk")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.motif.resources")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.motif")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.nimbus")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.windows.resources")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing.plaf.windows")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java.swing")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.java_cup.internal.runtime")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.javadoc")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.defaults")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.interceptor")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.mbeanserver")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.remote.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.remote.protocol.iiop")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.remote.protocol.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.remote.security")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.remote.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.IPAcl")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.agent")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.daemon")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.defaults")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.mpm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp.tasks")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.jmx.snmp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.naming.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.bcel.internal.classfile")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.bcel.internal.generic")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.bcel.internal.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.bcel.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.regexp.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.extensions")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.lib")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.res")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.templates")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.utils")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xslt")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.cmdline.getopt")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.cmdline")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.compiler.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.compiler")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.runtime.output")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.runtime")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.trax")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal.xsltc")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xalan.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dtd.models")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dtd")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dv.dtd")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dv.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dv.xs")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.dv")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.io")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.msg")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.validation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xpath.regex")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xpath")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs.identity")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs.models")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs.opti")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs.traversers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl.xs")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.impl")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.jaxp.datatype")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.jaxp.validation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.jaxp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.parsers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.utils")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xinclude")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xni.grammars")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xni.parser")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xni")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xpointer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xs.datatypes")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xerces.internal.xs")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.dtm.ref.dom2dtm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.dtm.ref.sax2dtm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.dtm.ref")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.dtm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.res")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.resolver.helpers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.resolver.readers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.resolver.tools")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.resolver")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.algorithms.implementations")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.algorithms")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.c14n.helper")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.c14n.implementations")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.c14n")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.encryption")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.exceptions")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.keys.keyresolver.implementations")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.keys.keyresolver")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.keys")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.signature.reference")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.signature")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.transforms.implementations")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.transforms.params")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.transforms")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security.utils")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.security")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.serialize")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.serializer.utils")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.serializer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.utils.res")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xml.internal.utils")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.axes")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.compiler")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.domapi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.functions")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.jaxp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.objects")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.operations")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.patterns")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal.res")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.org.apache.xpath.internal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.security.auth.callback")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.security.auth.login")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.security.auth.module")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.security.auth")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.security.jgss")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.source.doctree")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.source.tree")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.sun.source.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("com.x.logging")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.applet")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.color")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.datatransfer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.dnd.peer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.dnd")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.event")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.font")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.geom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.im.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.im")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.image.renderable")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.image")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.peer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt.print")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.awt")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.beans.beancontext")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.beans")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.io")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.annotation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.instrument")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.invoke")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.management")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.ref")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang.reflect")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.lang")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.math")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.net")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.channels.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.channels")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.charset.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.charset")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.file.attribute")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.file.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio.file")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.nio")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.rmi.activation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.rmi.dgc")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.rmi.registry")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.rmi.server")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.security.acl")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.security.cert")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.security.interfaces")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.security.spec")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.security")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.sql")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.text.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.text")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.time.chrono")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.time.format")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.time.temporal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.time.zone")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.time")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.concurrent.atomic")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.concurrent.locks")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.concurrent")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.function")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.jar")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.logging")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.prefs")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.regex")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.stream")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util.zip")
	J9_DECLARE_CONSTANT_UTF8_VALUE("java.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.accessibility")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.annotation.processing")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.annotation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.event")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.metadata")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.plugins.bmp")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.plugins.jpeg")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio.stream")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.imageio")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.lang.model.element")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.lang.model.type")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.lang.model.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.lang.model")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.loading")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.modelmbean")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.monitor")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.openmbean")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.relation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.remote.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.remote")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management.timer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.management")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.naming.directory")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.naming.event")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.naming.ldap")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.naming.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.naming")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.print.attribute.standard")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.print.attribute")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.print.event")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.print")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.rcm.management")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.rcm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.rmi.CORBA")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.rmi.ssl")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.script")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth.callback")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth.kerberos")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth.login")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth.x500")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.auth")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.cert")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.security.sasl")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sound.midi.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sound.midi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sound.sampled.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sound.sampled")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sql.rowset.serial")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sql.rowset.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sql.rowset")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.sql")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.border")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.colorchooser")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.event")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.filechooser")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf.basic")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf.metal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf.multi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf.nimbus")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf.synth")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.plaf")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.table")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.text.html.parser")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.text.html")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.text.rtf")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.text")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.tree")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing.undo")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.swing")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.tools")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind.annotation.adapters")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind.annotation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind.attachment")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind.helpers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.bind")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto.dsig.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto.dsig.keyinfo")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto.dsig.spec")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto.dsig")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.crypto")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.datatype")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.namespace")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.parsers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.soap")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.stream.events")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.stream.util")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.stream")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.transform.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.transform.sax")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.transform.stax")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.transform.stream")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.transform")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.validation")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.handler.soap")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.handler")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.http")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.soap")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.spi.http")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.spi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws.wsaddressing")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.ws")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml.xpath")
	J9_DECLARE_CONSTANT_UTF8_VALUE("javax.xml")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.apache.harmony.kernel.vm")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.ietf.jgss")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA.DynAnyPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA.ORBPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA.TypeCodePackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA.portable")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA_2_3.portable")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CORBA_2_3")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CosNaming.NamingContextExtPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CosNaming.NamingContextPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.CosNaming")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.Dynamic")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.DynamicAny.DynAnyFactoryPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.DynamicAny.DynAnyPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.DynamicAny")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.IOP.CodecFactoryPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.IOP.CodecPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.IOP")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.Messaging")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableInterceptor.ORBInitInfoPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableInterceptor")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer.CurrentPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer.POAManagerPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer.POAPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer.ServantLocatorPackage")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer.portable")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.PortableServer")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.SendingContext")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.omg.stub.java.rmi")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.bootstrap")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.css")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.events")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.html")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.ls")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.ranges")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.stylesheets")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.traversal")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.views")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom.xpath")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.w3c.dom")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.xml.sax.ext")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.xml.sax.helpers")
	J9_DECLARE_CONSTANT_UTF8_VALUE("org.xml.sax")
	J9_DECLARE_CONSTANT_UTF8_VALUE("sun.reflect")
};


#define JAVA8_PACKAGES_LEN 426
