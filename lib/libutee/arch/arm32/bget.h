/*
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ST BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*

    Interface definitions for bget.c, the memory management package.

*/

#ifndef _
#ifdef PROTOTYPES
#define  _(x)  x		/* If compiler knows prototypes */
#else
#define  _(x)  ()		/* It it doesn't */
#endif /* PROTOTYPES */
#endif

typedef long bufsize;
void bpool _((void *buffer, bufsize len));
void *bget _((bufsize size));
void *bgetz _((bufsize size));
void *bgetr _((void *buffer, bufsize newsize));
void brel _((void *buf));
void bectl _((int (*compact) (bufsize sizereq, int sequence),
	      void *(*acquire) (bufsize size),
	      void (*release) (void *buf), bufsize pool_incr));
void bstats _((bufsize *curalloc, bufsize *totfree, bufsize *maxfree,
	       long *nget, long *nrel));
void bstatse _((bufsize *pool_incr, long *npool, long *npget,
		long *nprel, long *ndget, long *ndrel));
void bufdump _((void *buf));
void bpoold _((void *pool, int dumpalloc, int dumpfree));
int bpoolv _((void *pool));
