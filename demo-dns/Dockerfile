# demo-dns is a DNS master server


FROM arpa2:build-bin AS src

RUN cd /usr/local/src ; git clone https://gitlab.labs.nic.cz/knot/knot-dns knot.git ; sed 's/@python_libknot_VERSION@/git.version/' < knot.git/python/libknot/__init__.py.in > knot.git/python/libknot/__init__.py

FROM arpa2:build-pip AS pip

RUN pip install cmdparser

FROM arpa2:base

RUN apt-get install -y gnupg python-six python-enum34 apt-transport-https ca-certificates

COPY knot.gpg /root/
COPY knot.list /etc/apt/sources.list.d/
RUN apt-key add /root/knot.gpg

RUN apt-get update && apt-get -y install knot knot-dnsutils

RUN ( echo ; echo 'control:' ; echo '    timeout: 0' ; echo ) >> /etc/knot/knot.conf

COPY README.MD /DNSmaster.md

RUN ln -s /usr/lib/x86_64-linux-gnu/libknot.so.8 /usr/lib/x86_64-linux-gnu/libknot.so
RUN ldconfig

COPY --from=pip /usr/local/lib/python2.7/dist-packages/cmdparser /usr/local/lib/python2.7/dist-packages/cmdparser

# Knot's Python library is not yet part of the distribution
#
COPY --from=src /usr/local/src/knot.git/python/ /usr/local/lib/python2.7/dist-packages/

COPY tightknot.py /usr/local/lib/python2.7/dist-packages/
COPY arpa2dns.py /usr/local/lib/python2.7/dist-packages/
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2dns.py /usr/bin/arpa2dns
RUN chmod ugo+x /usr/bin/arpa2dns

#TODO# Test
COPY test.py /

CMD ["bash"]
